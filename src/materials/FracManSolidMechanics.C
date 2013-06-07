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

#include "FracManSolidMechanics.h"

template<>
InputParameters validParams<FracManSolidMechanics>()
{
  InputParameters params= validParams<FracManPorousMedia>();

    
    params.addParam<Real>("matrix_thermal_expansion",1.0e-6,"thermal expansion coefficient of matrix (1/K)");
    params.addParam<Real>("matrix_youngs_modulus",1.50e10,"youngs modulus of matrix (Pa)");
    params.addParam<Real>("matrix_t_ref",293.15,"temperature of matrix where there is zero thermal strain (C)");
  
    params.addParam<Real>("poissons_ratio",0.2,"dimensionless");
    params.addParam<Real>("biot_coeff",1.0,"dimensionless");
    params.addParam<Real>("biot_modulus",2.5e10,"dimensionless");
    
    params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from the FracMan file");
    params.addRequiredParam<std::vector<Real> >("fracture_thermal_expansion","thermal expansion coefficient values associated with each of the fractures (1/K)");
    params.addRequiredParam<std::vector<Real> >("fracture_youngs_modulus","youngs modulus values associated with each of the fractures (Pa)");
    params.addRequiredParam<std::vector<Real> >("fracture_t_ref","temperature of fractures where there is zero thermal strain (C)");

    params.addCoupledVar("temperature", "coupled non-linear termperature variable");
    params.addCoupledVar("x_disp", "coupled non-linear x-disp variable");
    params.addCoupledVar("y_disp", "coupled non-linear y-disp variable");
    params.addCoupledVar("z_disp", "coupled non-linear z-disp variable");

  return params;

}

FracManSolidMechanics::FracManSolidMechanics(const std::string & name,
                               InputParameters parameters)
  :FracManPorousMedia(name, parameters),
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

    _matrix_thermal_expansion(getParam<Real>("matrix_thermal_expansion")),
    _matrix_youngs_modulus(getParam<Real>("matrix_youngs_modulus")),
    _matrix_t_ref(getParam<Real>("matrix_t_ref")),

    _fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),
    _fracture_thermal_expansion_vec(getParam<std::vector<Real> >("fracture_thermal_expansion")),
    _fracture_youngs_modulus_vec(getParam<std::vector<Real> >("fracture_youngs_modulus")),
    _fracture_t_ref_vec(getParam<std::vector<Real> >("fracture_t_ref")),

    _input_poissons_ratio(getParam<Real>("poissons_ratio")),
    _input_biot_coeff(getParam<Real>("biot_coeff")),
    _input_biot_modulus(getParam<Real>("biot_modulus")),

    //declare material properties
    _thermal_strain(declareProperty<Real>("thermal_strain")),
    _alpha(declareProperty<Real>("alpha")),
    _youngs_modulus(declareProperty<Real>("youngs_modulus")),
    _poissons_ratio(declareProperty<Real>("poissons_ratio")),
    _biot_coeff(declareProperty<Real>("biot_coeff")),
    _biot_modulus(declareProperty<Real>("biot_modulus")),

    _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
    _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
    _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
    _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector"))

{
    num_frac_vec_entries = _fracture_number_vec.size();
    num_te_vec_entries = _fracture_thermal_expansion_vec.size();
    num_ym_vec_entries = _fracture_youngs_modulus_vec.size();
    num_tref_vec_entries = _fracture_t_ref_vec.size();
}

void
FracManSolidMechanics::computeProperties()
{
  if (!areParentPropsComputed())
    FracManPorousMedia::computeProperties();

for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      if (_fracture_map[qp] == 0)
      {
          _youngs_modulus[qp]       = _matrix_youngs_modulus;
          _alpha[qp]                = _matrix_thermal_expansion;
          _thermal_strain[qp] = _matrix_thermal_expansion * (_temperature[qp] - _matrix_t_ref);
      }
      for (unsigned int k = 0; k < num_frac_vec_entries; k++)
      {
          if (_fracture_map[qp] == _fracture_number_vec[k])
          {
              if (num_ym_vec_entries < 2)
                  _youngs_modulus[qp] = _fracture_youngs_modulus_vec[0];
              else
                  _youngs_modulus[qp]         = _fracture_youngs_modulus_vec[k];
              
              if (num_te_vec_entries <2)
                  _material_porosity[qp] = _fracture_thermal_expansion_vec[0];
              else
                  _material_porosity[qp]    = _fracture_thermal_expansion_vec[k];
           }
       }
      
      if(_has_temp)
      {
          if (_fracture_map[qp] == 0)
              _thermal_strain[qp] = _matrix_thermal_expansion * (_temperature[qp] - _matrix_t_ref);

          for (unsigned int k = 0; k < num_frac_vec_entries; k++)
          {
              if (_fracture_map[qp] == _fracture_number_vec[k])
              {
                  if ((num_te_vec_entries < 2) | (num_tref_vec_entries <2))
                      _thermal_strain[qp] = _fracture_thermal_expansion_vec[0] * (_temperature[qp] - _fracture_t_ref_vec[0]);
                  else
                      _thermal_strain[qp] = _fracture_thermal_expansion_vec[k] * (_temperature[qp] - _fracture_t_ref_vec[k]);
              }
          }
      }
      else
          _thermal_strain[qp] = 0.0;

      _poissons_ratio[qp]   = _input_poissons_ratio;
      _biot_coeff[qp]       = _input_biot_coeff;
      _biot_modulus[qp]     = _input_biot_modulus;

// first try for strain and stress vectors
    if (_has_x_disp && _has_y_disp)
    {
      _E  =  _youngs_modulus[qp];
      _nu =  _poissons_ratio[qp];
      _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
      _c2 = _nu/(1.-_nu);
      _c3 = 0.5*(1.-2.*_nu)/(1.-_nu);

      _strain_normal_vector[qp](0) = _grad_x_disp[qp](0); //s_xx
      _strain_normal_vector[qp](1) = _grad_y_disp[qp](1); //s_yy
      if (_has_z_disp) _strain_normal_vector[qp](2) = _grad_z_disp[qp](2); //s_zz

      _strain_shear_vector[qp](0) = 0.5*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0)); // s_xy

      if (_has_z_disp)
      {
        _strain_shear_vector[qp](1) = 0.5*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0)); // s_xz
        _strain_shear_vector[qp](2) = 0.5*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1)); // s_yz
      }

        _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
        _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
        if (_has_z_disp) _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz

        _stress_shear_vector[qp](0) = _c1*_c3*2.0*_strain_shear_vector[qp](0); //tau_xy
        if (_has_z_disp)
        {
          _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
          _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
        }
    }
  }
}

void
FracManSolidMechanics::rotateSymmetricTensor( const ColumnMajorMatrix & R,
                                      const RealTensorValue & T,
                                      RealTensorValue & result )
{

  //     R           T         Rt
  //  00 01 02   00 01 02   00 10 20
  //  10 11 12 * 10 11 12 * 01 11 21
  //  20 21 22   20 21 22   02 12 22
  //
  if ( _dim == 3)
  {

    const Real T00 = R(0,0)*T(0,0) + R(0,1)*T(1,0) + R(0,2)*T(2,0);
    const Real T01 = R(0,0)*T(0,1) + R(0,1)*T(1,1) + R(0,2)*T(2,1);
    const Real T02 = R(0,0)*T(0,2) + R(0,1)*T(1,2) + R(0,2)*T(2,2);

    const Real T10 = R(1,0)*T(0,0) + R(1,1)*T(1,0) + R(1,2)*T(2,0);
    const Real T11 = R(1,0)*T(0,1) + R(1,1)*T(1,1) + R(1,2)*T(2,1);
    const Real T12 = R(1,0)*T(0,2) + R(1,1)*T(1,2) + R(1,2)*T(2,2);

    const Real T20 = R(2,0)*T(0,0) + R(2,1)*T(1,0) + R(2,2)*T(2,0);
    const Real T21 = R(2,0)*T(0,1) + R(2,1)*T(1,1) + R(2,2)*T(2,1);
    const Real T22 = R(2,0)*T(0,2) + R(2,1)*T(1,2) + R(2,2)*T(2,2);

    result = RealTensorValue(
      T00 * R(0,0) + T01 * R(0,1) + T02 * R(0,2),
      T00 * R(1,0) + T01 * R(1,1) + T02 * R(1,2),
      T00 * R(2,0) + T01 * R(2,1) + T02 * R(2,2),

      T10 * R(0,0) + T11 * R(0,1) + T12 * R(0,2),
      T10 * R(1,0) + T11 * R(1,1) + T12 * R(1,2),
      T10 * R(2,0) + T11 * R(2,1) + T12 * R(2,2),

      T20 * R(0,0) + T21 * R(0,1) + T22 * R(0,2),
      T20 * R(1,0) + T21 * R(1,1) + T22 * R(1,2),
      T20 * R(2,0) + T21 * R(2,1) + T22 * R(2,2) );
  }
  else if ( _dim == 2)
  {
    const Real T00 = R(0,0)*T(0,0) + R(0,1)*T(1,0);
    const Real T01 = R(0,0)*T(0,1) + R(0,1)*T(1,1) ;

    const Real T10 = R(1,0)*T(0,0) + R(1,1)*T(1,0) ;
    const Real T11 = R(1,0)*T(0,1) + R(1,1)*T(1,1) ;

    result = RealTensorValue(
      T00 * R(0,0) + T01 * R(0,1),
      T00 * R(1,0) + T01 * R(1,1),

      T10 * R(0,0) + T11 * R(0,1),
      T10 * R(1,0) + T11 * R(1,1));
  }
}