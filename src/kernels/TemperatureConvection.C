#include "Material.h"
#include "TemperatureConvection.h"

TemperatureConvection::TemperatureConvection(std::string name,
                                             Parameters parameters,
                                             std::string var_name,
                                             std::vector<std::string> coupled_to,
                                             std::vector<std::string> coupled_as)
  :Kernel(name,parameters,var_name,true,coupled_to,coupled_as)
{}

void
TemperatureConvection::subdomainSetup()
{
   _water_specific_heat = &_material->getRealProperty("water_specific_heat");
   _darcy_velocity =  &_material->getGradientProperty("darcy_velocity");
}

Real TemperatureConvection::computeQpResidual()
{
  return (*_water_specific_heat)[_qp]*(*_darcy_velocity)[_qp]*_phi[_i][_qp]*_grad_u[_qp];
}

Real TemperatureConvection::computeQpJacobian()
{
  return (*_water_specific_heat)[_qp]*(*_darcy_velocity)[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
}
