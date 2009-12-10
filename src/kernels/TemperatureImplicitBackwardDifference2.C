#include "TemperatureImplicitBackwardDifference2.h"
#include "Material.h"

TemperatureImplicitBackwardDifference2::TemperatureImplicitBackwardDifference2(std::string name,
                                       InputParameters parameters,
                                       std::string var_name,
                                       std::vector<std::string> coupled_to,
                                       std::vector<std::string> coupled_as)
  :ImplicitBackwardDifference2(name,parameters,var_name,coupled_to,coupled_as)
{}

void
TemperatureImplicitBackwardDifference2::subdomainSetup()
{
  _water_specific_heat = &_material->getRealProperty("water_specific_heat");
  _rock_specific_heat = &_material->getRealProperty("rock_specific_heat");
  _porosity = &_material->getRealProperty("porosity");
  _rho_w = &_material->getRealProperty("rho_w");
  _rho_r = &_material->getRealProperty("rho_r");

}

Real
TemperatureImplicitBackwardDifference2::computeQpResidual()
{
  return (((*_porosity)[_qp]*(*_rho_w)[_qp]*(*_water_specific_heat)[_qp])+((1-(*_porosity)[_qp])*(*_rho_r)[_qp]*(*_rock_specific_heat)[_qp]))*ImplicitBackwardDifference2::computeQpResidual();
}

Real
TemperatureImplicitBackwardDifference2::computeQpJacobian()
{
  return (((*_porosity)[_qp]*(*_rho_w)[_qp]*(*_water_specific_heat)[_qp])+((1-(*_porosity)[_qp])*(*_rho_r)[_qp]*(*_rock_specific_heat)[_qp]))*ImplicitBackwardDifference2::computeQpJacobian();
}
