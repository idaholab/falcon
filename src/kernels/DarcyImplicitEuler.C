#include "DarcyImplicitEuler.h"
#include "Material.h"

DarcyImplicitEuler::DarcyImplicitEuler(std::string name,
                                       InputParameters parameters,
                                       std::string var_name,
                                       std::vector<std::string> coupled_to,
                                       std::vector<std::string> coupled_as)
  :ImplicitEuler(name,parameters,var_name,coupled_to,coupled_as)
{}

void
DarcyImplicitEuler::subdomainSetup()
{
  _c_f = &_material->getRealProperty("c_f");
  _porosity = &_material->getRealProperty("porosity");
  _rho_w = &_material->getRealProperty("rho_w");
}

Real
DarcyImplicitEuler::computeQpResidual()
{
  return (*_c_f)[_qp]*(*_porosity)[_qp]*(*_rho_w)[_qp]*ImplicitEuler::computeQpResidual();
}

Real
DarcyImplicitEuler::computeQpJacobian()
{
  return (*_c_f)[_qp]*(*_porosity)[_qp]*(*_rho_w)[_qp]*ImplicitEuler::computeQpJacobian();
}
