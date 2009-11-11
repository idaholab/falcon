#include "Material.h"
#include "DarcyMassFluxZ.h"

DarcyMassFluxZ::DarcyMassFluxZ(std::string name,
                               InputParameters parameters,
                               std::string var_name,
                               std::vector<std::string> coupled_to,
                               std::vector<std::string> coupled_as)
  :Kernel(name,parameters,var_name,true,coupled_to,coupled_as)
{}

void
DarcyMassFluxZ::subdomainSetup()
{
  _darcy_params = &_material->getRealProperty("darcy_params");
  _gravity = &_material->getRealProperty("gravity");
  _rho_w = &_material->getRealProperty("rho_w");
  
  _gravity_vector = &_material->getRealVectorValueProperty("gravity_vector");
}

Real
DarcyMassFluxZ::computeQpResidual()
{
  return (*_darcy_params)[_qp]*(*_gravity)[_qp]*(*_rho_w)[_qp]*(*_gravity_vector)[_qp]*_dphi[_i][_qp];
}

Real
DarcyMassFluxZ::computeQpJacobian()
{
return 0;
}
