#include "SolidMechanics.h"

template<>
InputParameters validParams<SolidMechanics>()
{
  InputParameters params= validParams<PorousMedia>();
  params.addParam<Real>("thermal_expansion",1.0e-6,"thermal expansion coefficient (1/K)");
  params.addParam<Real>("youngs_modulus",1.50e10,"in Pascal") ;
  params.addParam<Real>("poissons_ratio",0.2,"dimensionless");
  params.addParam<Real>("biot_coeff",0.0,"dimensionless");
  params.addParam<Real>("t_ref",293.15,"initial temperature");

  //RKP added 10-18-2010
  params.addCoupledVar("temperature", "TODO:  add description");//Hai forgot this
  params.addCoupledVar("x_disp", "TODO: ad description");
  params.addCoupledVar("y_disp", "TODO: ad description");
  params.addCoupledVar("z_disp", "TODO: ad description");
  // params.addCoupledVar("pressure", "TODO:  add description");//Hai forgot this
  return params;
  
}

SolidMechanics::SolidMechanics(const std::string & name,
                               MooseSystem & moose_system,
                               InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),
     _has_temp(isCoupled("temperature")),
     _grad_temp  (_has_temp ? coupledGradient("temperature") : _grad_zero),
     _temperature(_has_temp ? coupledValue("temperature")  : _zero),
      _has_x_disp(isCoupled("x_disp")),
     _grad_x_disp(_has_x_disp ? coupledGradient("x_disp") : _grad_zero),
      _has_y_disp(isCoupled("y_disp")),
     _grad_y_disp(_has_y_disp ? coupledGradient("y_disp") : _grad_zero),
      _has_z_disp(isCoupled("z_disp")),
     _grad_z_disp(_has_z_disp ? coupledGradient("z_disp") : _grad_zero),
     
     _input_thermal_expansion(getParam<Real>("thermal_expansion")),
     _input_youngs_modulus(getParam<Real>("youngs_modulus")),
     _input_poissons_ratio(getParam<Real>("poissons_ratio")),
     _input_biot_coeff(getParam<Real>("biot_coeff")),     
     _input_t_ref(getParam<Real>("t_ref")),
     //declare material properties
     _thermal_strain(declareProperty<Real>("thermal_strain")),
     _alpha(declareProperty<Real>("alpha")),
     _youngs_modulus(declareProperty<Real>("youngs_modulus")),
     _poissons_ratio(declareProperty<Real>("poissons_ratio")),
     _biot_coeff(declareProperty<Real>("biot_coeff")),
     
     _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
     _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector")),
     _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
     _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector"))
{ }

void
SolidMechanics::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {

    _alpha[qp]                = _input_thermal_expansion;
    
    if(_has_temp)
      _thermal_strain[qp] = _input_thermal_expansion*(_temperature[qp] - _input_t_ref);
    else
      _thermal_strain[qp] = 0.0;
    
    _youngs_modulus[qp]   = _input_youngs_modulus;
    _poissons_ratio[qp]   = _input_poissons_ratio;
    _biot_coeff[qp]       = _input_biot_coeff;

    if (_has_x_disp && _has_y_disp)
    {
      
      _E  =  _youngs_modulus[qp];
      _nu =  _poissons_ratio[qp];
      _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
      _c2 = _nu/(1.-_nu);
      _c3 = 0.5*(1.-2.*_nu)/(1.-_nu);
      
      _strain_normal_vector[qp](0) = _grad_x_disp[qp](0); //s_xx
      _strain_normal_vector[qp](1) = _grad_y_disp[qp](1); //s_yy
      if (_dim == 3)
        _strain_normal_vector[qp](2) = _grad_z_disp[qp](2); //s_zz

      _strain_shear_vector[qp](0) = 0.5*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0)); // s_xy
      if (_dim == 3)
      {
        _strain_shear_vector[qp](1) = 0.5*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0)); // s_xz
        _strain_shear_vector[qp](2) = 0.5*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1)); // s_yz
      }
      
      _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
      _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
      if (_dim == 3)
        _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz

      _stress_shear_vector[qp](0) = _c1*_c3*2.0*_strain_shear_vector[qp](0); //tau_xy
      if (_dim == 3)
      {
        _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
        _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
      }
      
    }
  }
}
