#include "Material.h"
#include "DarcyMassFluxPressure.h"

DarcyMassFluxPressure::DarcyMassFluxPressure(std::string name,
                                             InputParameters parameters,
                                             std::string var_name,
                                             std::vector<std::string> coupled_to,
                                             std::vector<std::string> coupled_as)
  :Diffusion(name,parameters,var_name,coupled_to,coupled_as)
{}

void
DarcyMassFluxPressure::subdomainSetup()
{
  _darcy_params = &_material->getRealProperty("darcy_params");
}

Real
DarcyMassFluxPressure::computeQpResidual()
{
  return (*_darcy_params)[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressure::computeQpJacobian()
{
  return (*_darcy_params)[_qp]*Diffusion::computeQpJacobian();
}
