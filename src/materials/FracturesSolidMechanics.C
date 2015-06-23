/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "FracturesSolidMechanics.h"

template<>
InputParameters validParams<FracturesSolidMechanics>()
{
  InputParameters params= validParams<FracturesPorousMedia>();
  // General
  // coupled variables
  params.addCoupledVar("temperature", "Coupled non-linear termperature variable, [K]");
  params.addCoupledVar("x_disp", "Coupled non-linear x-disp variable, [m]");
  params.addCoupledVar("y_disp", "Coupled non-linear y-disp variable, [m]");
  params.addCoupledVar("z_disp", "Coupled non-linear z-disp variable. [m]");
  // rock property inputs
  params.addParam<Real>("poissons_ratio", 0.2, "dimensionless");
  params.addParam<Real>("biot_coeff", 1.0, "dimensionless");
  params.addParam<Real>("biot_modulus", 2.5e10, "dimensionless");
  // Matrix
  params.addParam<Real>("matrix_thermal_expansion", 1.0e-6, "thermal expansion coefficient of matrix, [1/K]");
  params.addParam<Real>("matrix_youngs_modulus", 1.50e10, "youngs modulus of matrix, [Pa]");
  params.addParam<Real>("matrix_thermal_strain_ref_temp", 293.15, "Initial reference temperature of matrix where there is no thermal strain, [K]");
  // Fractures
  params.addParam<Real>("fracture_thermal_expansion", 1.0e-6, "thermal expansion coefficient of fractures, [1/K]");
  params.addParam<Real>("fracture_youngs_modulus", 1.50e10, "youngs modulus of fractures, [Pa]");
  params.addParam<Real>("fracture_thermal_strain_ref_temp", 293.15, "Initial reference temperature of fractures where there is no thermal strain, [K]");
  return params;
}

FracturesSolidMechanics::FracturesSolidMechanics(const std::string & name,
                                                 InputParameters parameters) :
    FracturesPorousMedia(name, parameters),
    // Grab coupled variables
    _has_temp(isCoupled("temperature")),
    _temperature(_has_temp ? coupledValue("temperature")  : _zero),
    _has_x_disp(isCoupled("x_disp")),
    _grad_x_disp(_has_x_disp ? coupledGradient("x_disp") : _grad_zero),
    _grad_x_disp_old(_has_x_disp && _fe_problem.isTransient() ? coupledGradientOld("x_disp") : _grad_zero),
    _has_y_disp(isCoupled("y_disp")),
    _grad_y_disp(_has_y_disp ? coupledGradient("y_disp") : _grad_zero),
    _grad_y_disp_old(_has_y_disp && _fe_problem.isTransient() ? coupledGradientOld("y_disp") : _grad_zero),
    _has_z_disp(isCoupled("z_disp")),
    _grad_z_disp(_has_z_disp ? coupledGradient("z_disp") : _grad_zero),
    _grad_z_disp_old(_has_z_disp && _fe_problem.isTransient() ? coupledGradientOld("z_disp") : _grad_zero),

    // Grab user input parameters
    // General
    _input_poissons_ratio(getParam<Real>("poissons_ratio")),
    _input_biot_coeff(getParam<Real>("biot_coeff")),
    _input_biot_modulus(getParam<Real>("biot_modulus")),

    // Matrix
    _matrix_thermal_expansion(getParam<Real>("matrix_thermal_expansion")),
    _matrix_youngs_modulus(getParam<Real>("matrix_youngs_modulus")),
    _matrix_t_ref(getParam<Real>("matrix_thermal_strain_ref_temp")),

    // Fractures
    _fracture_thermal_expansion(getParam<Real>("fracture_thermal_expansion")),
    _fracture_youngs_modulus(getParam<Real>("fracture_youngs_modulus")),
    _fracture_t_ref(getParam<Real>("fracture_thermal_strain_ref_temp")),

    // Declare material properties
    // Rock material props
    _thermal_strain(declareProperty<Real>("thermal_strain")),
    _alpha(declareProperty<Real>("alpha")),
    _youngs_modulus(declareProperty<Real>("youngs_modulus")),
    _poissons_ratio(declareProperty<Real>("poissons_ratio")),
    _biot_coeff(declareProperty<Real>("biot_coeff")),
    _biot_modulus(declareProperty<Real>("biot_modulus")),
    // Stress/strain material props
    _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
    _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
    _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
    _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector")),

    // Grab darcy_flux_water_old stateful material property from FracturesFluidFlow to calculate strain dependent permeability mod
    _darcy_flux_water_old(_has_strain_dependent_permeability ? &getMaterialPropertyOldByName<RealGradient>("darcy_flux_water") : NULL)
{
}

void
FracturesSolidMechanics::computeProperties()
{
  if (!areParentPropsComputed())
    FracturesPorousMedia::computeProperties();

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // calculating/assigning rock material properties:

    // material property assignment for the fractures
    if (_fractures[_qp] == _fracture_num)
    {
      _youngs_modulus[_qp] = _fracture_youngs_modulus;
      _alpha[_qp] = _fracture_thermal_expansion;

      if(_has_temp)
        _thermal_strain[_qp] = _fracture_thermal_expansion * (_temperature[_qp] - _fracture_t_ref);
      else
        _thermal_strain[_qp] = 0.0;
    }
    // material property assignment for matrix
    else
    {
      _youngs_modulus[_qp] = _matrix_youngs_modulus;
      _alpha[_qp] = _matrix_thermal_expansion;

      if(_has_temp)
        _thermal_strain[_qp] = _matrix_thermal_expansion * (_temperature[_qp] - _matrix_t_ref);
      else
        _thermal_strain[_qp] = 0.0;
    }

    // general material property assignment
    _poissons_ratio[_qp]   = _input_poissons_ratio;
    _biot_coeff[_qp]       = _input_biot_coeff;
    _biot_modulus[_qp]     = _input_biot_modulus;

    // calculating/assigning stress/strain material properties:
    if (_has_x_disp && _has_y_disp)
    {
      E  = _youngs_modulus[_qp];
      nu = _poissons_ratio[_qp];
      c1 = E * (1.0 - nu) / (1.0 + nu) / (1.0 - 2.0 * nu);
      c2 = nu / (1.0 - nu);
      c3 = 0.5 * (1.0 - 2.0 * nu) / (1.0 - nu);

      // start with strain_normal (s_xx and s_yy)
      _strain_normal_vector[_qp](0) = _grad_x_disp[_qp](0);
      _strain_normal_vector[_qp](1) = _grad_y_disp[_qp](1);
      // if 3D problem, strain_normal in z_direction (s_zz)
      if (_has_z_disp)
        _strain_normal_vector[_qp](2) = _grad_z_disp[_qp](2);

      // next strain_shear (s_xy)
      _strain_shear_vector[_qp](0) = 0.5 * (_grad_x_disp[_qp](1) + _grad_y_disp[_qp](0));
      // if 3D problem, strain_shear in z_directions (s_xz and s_yz)
      if (_has_z_disp)
      {
        _strain_shear_vector[_qp](1) = 0.5 * (_grad_x_disp[_qp](2) + _grad_z_disp[_qp](0));
        _strain_shear_vector[_qp](2) = 0.5 * (_grad_y_disp[_qp](2) + _grad_z_disp[_qp](1));
      }

      // now stress_normal (tau_xx and tau_yy)
      _stress_normal_vector[_qp](0) = c1 * _strain_normal_vector[_qp](0) + c1 * c2 * _strain_normal_vector[_qp](1) + c1 * c2 * _strain_normal_vector[_qp](2);
      _stress_normal_vector[_qp](1) = c1 * c2 * _strain_normal_vector[_qp](0) + c1 * _strain_normal_vector[_qp](1) + c1 * c2 * _strain_normal_vector[_qp](2);
      // if 3D problem, stress_normal in z-direction (tau_zz)
      if (_has_z_disp)
        _stress_normal_vector[_qp](2) = c1 * c2 * _strain_normal_vector[_qp](0) + c1 * c2 * _strain_normal_vector[_qp](1) + c1 * _strain_normal_vector[_qp](2);

      // then stress_shear (tau_xy)
      _stress_shear_vector[_qp](0) = c1 * c3 * 2.0 * _strain_shear_vector[_qp](0);
      // if 3D problem, stress_shear in z_directions (tau_xz and tau_yz)
      if (_has_z_disp)
      {
        _stress_shear_vector[_qp](1) = c1 * c3 * 2.0 * _strain_shear_vector[_qp](1);
        _stress_shear_vector[_qp](2) = c1 * c3 * 2.0 * _strain_shear_vector[_qp](2);
      }
    }

    // calculating strain dependent permeability:
    if (_has_strain_dependent_permeability)
    {
      // Determining magnitude of aperture change (ie. strain perpandicular to fracture surface)/

      // determining direction of fluid flow (which is parallel with fracture direction)
      Real vx = (*_darcy_flux_water_old)[_qp](0);
      Real vy = (*_darcy_flux_water_old)[_qp](1);

      // finding vector perpendicular to fluid flow
      Real ux = std::abs((1/std::sqrt(vx*vx + vy*vy))*vy);
      Real uy = std::abs((1/std::sqrt(vx*vx + vy*vy))*vx);

      // finding strain vector perpendicular to fluid flow
      Real fracture_strain_normal_x = ux * _strain_normal_vector[_qp](0);
      Real fracture_strain_normal_y = uy * _strain_normal_vector[_qp](1);

      // magnitude of strain perpandicular to fluid flow
      Real fracture_strain_normal = fracture_strain_normal_x + fracture_strain_normal_y;

      Real aperture = std::sqrt(12 * _fracture_permeability);
      Real fracture_ratio = _model_fracture_aperture / aperture;

      if (_fractures[_qp] == _fracture_num)
      {
        if (_t_step == 1)
          _permeability[_qp] = ((std::pow(_model_fracture_aperture, 2)) * (std::pow((1/fracture_ratio), 3)))/12;
        else
          _permeability[_qp] = ((std::pow(_model_fracture_aperture, 2)) * (std::pow(((1/fracture_ratio) + fracture_strain_normal), 3)))/12;

        if (_permeability[_qp] <= (0.9 * (std::pow(aperture, 3) / (12 * _model_fracture_aperture))))
          _permeability[_qp] = 0.9 * (std::pow(aperture, 3) / (12 * _model_fracture_aperture));
      }
      else
        _permeability[_qp] = _matrix_permeability;
    }
  }
}
