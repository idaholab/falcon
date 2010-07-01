#include "DarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<DarcyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

DarcyImplicitEuler::DarcyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),
   _c_f(getMaterialProperty<Real>("c_f")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _rho_w(getMaterialProperty<Real>("rho_w"))
{}

Real
DarcyImplicitEuler::computeQpResidual()
{
  return _c_f[_qp]*_porosity[_qp]*_rho_w[_qp]*ImplicitEuler::computeQpResidual();
}

Real
DarcyImplicitEuler::computeQpJacobian()
{
  return _c_f[_qp]*_porosity[_qp]*_rho_w[_qp]*ImplicitEuler::computeQpJacobian();
}
