#include "Constant.h"

template<>
InputParameters validParams<Constant>()
{
  InputParameters params;
  params.set<Real>("thermal_conductivity")=1.0;
  params.set<Real>("thermal_expansion")=1.0;
  params.set<Real>("specific_heat")=1.0;
  params.set<Real>("density")=1.0;
  params.set<Real>("youngs_modulus")=1.0;
  params.set<Real>("poissons_ratio")=1.0;
  params.set<Real>("biot_coeff")=1.0;
  params.set<Real>("t_ref")=300;

  return params;
}

Constant::Constant(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as)
    :Material(name,parameters,block_id,coupled_to,coupled_as),
    _has_temp(isCoupled("temp")),
    _temp(_has_temp ? coupledVal("temp") : _zero),
    _my_thermal_conductivity(parameters.get<Real>("thermal_conductivity")),
    _my_thermal_expansion(parameters.get<Real>("thermal_expansion")),
    _my_specific_heat(parameters.get<Real>("specific_heat")),
    _my_density(parameters.get<Real>("density")),
    _my_youngs_modulus(parameters.get<Real>("youngs_modulus")),
    _my_poissons_ratio(parameters.get<Real>("poissons_ratio")),
    _my_biot_coeff(parameters.get<Real>("biot_coeff")),     
    _my_t_ref(parameters.get<Real>("t_ref")),
    _thermal_conductivity(declareRealProperty("thermal_conductivity")),
    _thermal_strain(declareRealProperty("thermal_strain")),
    _alpha(declareRealProperty("alpha")),
    _biot_coeff(declareRealProperty("biot_coeff")),     
    _specific_heat(declareRealProperty("specific_heat")),
    _density(declareRealProperty("density")),
    _youngs_modulus(declareRealProperty("youngs_modulus")),
    _poissons_ratio(declareRealProperty("poissons_ratio"))
  {}

void
Constant::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    _density[qp] = _my_density;
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
