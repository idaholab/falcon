#include "Material.h"
#include "TemperatureDiffusion.h"

TemperatureDiffusion::TemperatureDiffusion(std::string name,
                                             Parameters parameters,
                                             std::string var_name,
                                             std::vector<std::string> coupled_to,
                                             std::vector<std::string> coupled_as)
  :Diffusion(name,parameters,var_name,coupled_to,coupled_as)
{}


void
TemperatureDiffusion::subdomainSetup()
{
  _thermal_conductivity = &_material->getRealProperty("thermal_conductivity");
}

Real
TemperatureDiffusion::computeQpResidual()
{
  return (*_thermal_conductivity)[_qp]*Diffusion::computeQpResidual();
}

Real
TemperatureDiffusion::computeQpJacobian()
{
  return (*_thermal_conductivity)[_qp]*Diffusion::computeQpJacobian();
}
