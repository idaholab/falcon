#include "Constant.h"

template<>
InputParameters validParams<Constant>()
{
  InputParameters params = validParams<Material>();
  params.set<Real>("thermal_conductivity")=1.0;
  params.set<Real>("thermal_expansion")=1.0;
  params.set<Real>("specific_heat")=1.0;
  params.set<Real>("density")=1.0;
  params.set<Real>("rho_r")=1.0;
  params.set<Real>("youngs_modulus")=1.0;
  params.set<Real>("poissons_ratio")=1.0;
  params.set<Real>("biot_coeff")=1.0;
  params.set<Real>("t_ref")=300;

  return params;
}

Constant::Constant(const std::string & name,
                   MooseSystem & moose_system,
                   InputParameters parameters)
  :Material(name, moose_system, parameters),
     _has_temp(isCoupled("temp")),
     _temp(_has_temp ? coupledValue("temp") : _zero),
     _my_thermal_conductivity(getParam<Real>("thermal_conductivity")),
     _my_thermal_expansion(getParam<Real>("thermal_expansion")),
     _my_specific_heat(getParam<Real>("specific_heat")),
     _my_density(getParam<Real>("density")),
     _my_youngs_modulus(getParam<Real>("youngs_modulus")),
     _my_poissons_ratio(getParam<Real>("poissons_ratio")),
     _my_biot_coeff(getParam<Real>("biot_coeff")),     
     _my_t_ref(getParam<Real>("t_ref")),
     _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
     _thermal_strain(declareProperty<Real>("thermal_strain")),
     _alpha(declareProperty<Real>("alpha")),
     _biot_coeff(declareProperty<Real>("biot_coeff")),
     _specific_heat(declareProperty<Real>("specific_heat")),
     _density(declareProperty<Real>("density")),
     _rho_r(declareProperty<Real>("rho_r")),
     _youngs_modulus(declareProperty<Real>("youngs_modulus")),
     _poissons_ratio(declareProperty<Real>("poissons_ratio"))
  {}

void
Constant::computeProperties()
{
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {
    _density[qp] = _my_density;
    _rho_r[qp] = _my_density;
    _thermal_conductivity[qp] = _my_thermal_conductivity;
    _specific_heat[qp] = _my_specific_heat;
    _biot_coeff[qp] = _my_biot_coeff;    

    if(_has_temp)
      _thermal_strain[qp] = _my_thermal_expansion*(_temp[qp] - _my_t_ref);
    else
      _thermal_strain[qp] = 0;

    _alpha[qp] = _my_thermal_expansion;
    
    _youngs_modulus[qp]  = _my_youngs_modulus;
    _poissons_ratio[qp] = _my_poissons_ratio;
  }
}
