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

#include "StochasticSolidMechanics.h"

template<>
InputParameters validParams<StochasticSolidMechanics>()
{
  InputParameters params= validParams<StochasticPorousMedia>();
  //coupled variables
  params.addCoupledVar("temperature", "Coupled non-linear termperature variable, [K]");
  params.addCoupledVar("x_disp", "Coupled non-linear x-disp variable, [m]");
  params.addCoupledVar("y_disp", "Coupled non-linear y-disp variable, [m]");
  params.addCoupledVar("z_disp", "Coupled non-linear z-disp variable. [m]");

  //rock property inputs
  params.addParam<Real>("poissons_ratio",0.2,"dimensionless");
  params.addParam<Real>("biot_coeff",1.0,"dimensionless");
  params.addParam<Real>("biot_modulus",2.5e10,"dimensionless");
  params.addParam<Real>("thermal_expansion",1.0e-6,"thermal expansion coefficient, [1/K]");
  params.addParam<Real>("youngs_modulus",1.50e10,"youngs modulus of the material, [Pa]");
  params.addParam<Real>("thermal_strain_ref_temp",293.15,"Initial reference temperature where there is no thermal strain, [K]");

  return params;
}

StochasticSolidMechanics::StochasticSolidMechanics(const std::string & name,
                               InputParameters parameters)
  :StochasticPorousMedia(name, parameters),
////Grab coupled variables
    _has_temp(isCoupled("temperature")),
    _temperature(_has_temp ? coupledValue("temperature")  : _zero),
    _has_x_disp(isCoupled("x_disp")),
    _grad_x_disp(_has_x_disp ? coupledGradient("x_disp") : _grad_zero),
    _grad_x_disp_old(_has_x_disp ? coupledGradientOld("x_disp") : _grad_zero),
    _has_y_disp(isCoupled("y_disp")),
    _grad_y_disp(_has_y_disp ? coupledGradient("y_disp") : _grad_zero),
    _grad_y_disp_old(_has_y_disp ? coupledGradientOld("y_disp") : _grad_zero),
    _has_z_disp(isCoupled("z_disp")),
    _grad_z_disp(_has_z_disp ? coupledGradient("z_disp") : _grad_zero),
    _grad_z_disp_old(_has_z_disp ? coupledGradientOld("z_disp") : _grad_zero),

////Grab user input parameters
    _input_thermal_expansion(getParam<Real>("thermal_expansion")),
    _input_youngs_modulus(getParam<Real>("youngs_modulus")),
    _input_poissons_ratio(getParam<Real>("poissons_ratio")),
    _input_biot_coeff(getParam<Real>("biot_coeff")),
    _input_biot_modulus(getParam<Real>("biot_modulus")),
    _input_t_ref(getParam<Real>("thermal_strain_ref_temp")),

////Declare material properties
    //rock material props
    _thermal_strain(declareProperty<Real>("thermal_strain")),
    _alpha(declareProperty<Real>("alpha")),
    _youngs_modulus(declareProperty<Real>("youngs_modulus")),
    _poissons_ratio(declareProperty<Real>("poissons_ratio")),
    _biot_coeff(declareProperty<Real>("biot_coeff")),
    _biot_modulus(declareProperty<Real>("biot_modulus")),
    //stress/strain material props
    _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
    _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
    _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
    _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector"))

{}

void
StochasticSolidMechanics::computeProperties()
{
  if (!areParentPropsComputed())
    StochasticPorousMedia::computeProperties();

    for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
    {
//----------------------------------------------------------------------------------------------------------------------------//
////calculating/assigning rock material properties:

        //material property assignment for matrix
        _youngs_modulus[qp] = _input_youngs_modulus;
        _alpha[qp]            = _input_thermal_expansion;
        _poissons_ratio[qp]   = _input_poissons_ratio;
        _biot_coeff[qp]       = _input_biot_coeff;
        _biot_modulus[qp]     = _input_biot_modulus;
        //thermal strain calculation
        if(_has_temp)
            _thermal_strain[qp] = _input_thermal_expansion*(_temperature[qp] - _input_t_ref);
        else
            _thermal_strain[qp] = 0.0;

//----------------------------------------------------------------------------------------------------------------------------//
////calculating/assigning stress/strain material properties:

        if (_has_x_disp && _has_y_disp)
        {
            E  = _youngs_modulus[qp];
            nu = _poissons_ratio[qp];
            c1 = E * (1.0 - nu) / (1.0 + nu) / (1.0 - 2.0 * nu);
            c2 = nu / (1.0 - nu);
            c3 = 0.5 * (1.0 - 2.0 * nu) / (1.0 - nu);

            //start with strain_normal (s_xx and s_yy)
            _strain_normal_vector[qp](0) = _grad_x_disp[qp](0);
            _strain_normal_vector[qp](1) = _grad_y_disp[qp](1);
            //if 3D problem, strain_normal in z_direction (s_zz)
            if (_has_z_disp)
                _strain_normal_vector[qp](2) = _grad_z_disp[qp](2);

            //next strain_shear (s_xy)
            _strain_shear_vector[qp](0) = 0.5 * (_grad_x_disp[qp](1) + _grad_y_disp[qp](0));
            //if 3D problem, strain_shear in z_directions (s_xz and s_yz)
            if (_has_z_disp)
            {
                _strain_shear_vector[qp](1) = 0.5 * (_grad_x_disp[qp](2) + _grad_z_disp[qp](0));
                _strain_shear_vector[qp](2) = 0.5 * (_grad_y_disp[qp](2) + _grad_z_disp[qp](1));
            }

            //now stress_normal (tau_xx and tau_yy)
            _stress_normal_vector[qp](0) = c1 * _strain_normal_vector[qp](0) + c1 * c2 * _strain_normal_vector[qp](1) + c1 * c2 * _strain_normal_vector[qp](2);
            _stress_normal_vector[qp](1) = c1 * c2 * _strain_normal_vector[qp](0) + c1 * _strain_normal_vector[qp](1) + c1 * c2 * _strain_normal_vector[qp](2);
            //if 3D problem, stress_normal in z-direction (tau_zz)
            if (_has_z_disp)
                _stress_normal_vector[qp](2) = c1 * c2 * _strain_normal_vector[qp](0) + c1 * c2 * _strain_normal_vector[qp](1) + c1 * _strain_normal_vector[qp](2);

            //then stress_shear (tau_xy)
            _stress_shear_vector[qp](0) = c1 * c3 * 2.0 * _strain_shear_vector[qp](0);
            //if 3D problem, stress_shear in z_directions (tau_xz and tau_yz)
            if (_has_z_disp)
            {
                _stress_shear_vector[qp](1) = c1 * c3 * 2.0 * _strain_shear_vector[qp](1);
                _stress_shear_vector[qp](2) = c1 * c3 * 2.0 * _strain_shear_vector[qp](2);
            }
        }
    }
}
