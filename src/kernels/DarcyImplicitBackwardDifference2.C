#include "DarcyImplicitBackwardDifference2.h"
#include "Material.h"

template<>
InputParameters validParams<DarcyImplicitBackwardDifference2>()
{
  InputParameters params = validParams<ImplicitBackwardDifference2>();
  return params;
}

DarcyImplicitBackwardDifference2::DarcyImplicitBackwardDifference2(std::string name,
                                                                   MooseSystem & moose_system,
                                                                   InputParameters parameters)
  :ImplicitBackwardDifference2(name, moose_system, parameters)
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
