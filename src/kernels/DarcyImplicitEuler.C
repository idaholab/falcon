#include "DarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<DarcyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

DarcyImplicitEuler::DarcyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters)
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
