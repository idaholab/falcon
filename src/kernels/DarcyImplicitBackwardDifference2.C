#include "DarcyImplicitBackwardDifference2.h"
#include "Material.h"

DarcyImplicitBackwardDifference2::DarcyImplicitBackwardDifference2(std::string name,
                                       InputParameters parameters,
                                       std::string var_name,
                                       std::vector<std::string> coupled_to,
                                       std::vector<std::string> coupled_as)
  :ImplicitBackwardDifference2(name,parameters,var_name,coupled_to,coupled_as)
{}

void
DarcyImplicitBackwardDifference2::subdomainSetup()
{
  _c_f = &_material->getRealProperty("c_f");
  _porosity = &_material->getRealProperty("porosity");
  _rho_w = &_material->getRealProperty("rho_w");
}

Real
DarcyImplicitBackwardDifference2::computeQpResidual()
{
  return (*_c_f)[_qp]*(*_porosity)[_qp]*(*_rho_w)[_qp]*ImplicitBackwardDifference2::computeQpResidual();
}

Real
DarcyImplicitBackwardDifference2::computeQpJacobian()
{
  return (*_c_f)[_qp]*(*_porosity)[_qp]*(*_rho_w)[_qp]*ImplicitBackwardDifference2::computeQpJacobian();
}
