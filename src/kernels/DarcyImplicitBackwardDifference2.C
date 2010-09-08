#include "DarcyImplicitBackwardDifference2.h"
#include "Material.h"

template<>
InputParameters validParams<DarcyImplicitBackwardDifference2>()
{
  InputParameters params = validParams<ImplicitBackwardDifference2>();
  return params;
}

DarcyImplicitBackwardDifference2::DarcyImplicitBackwardDifference2(const std::string & name,
                                                                   MooseSystem & moose_system,
                                                                   InputParameters parameters)
  :ImplicitBackwardDifference2(name, moose_system, parameters),
   _c_f(getMaterialProperty<Real>("c_f")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _rho_w(getMaterialProperty<Real>("rho_w"))
{}

Real
DarcyImplicitBackwardDifference2::computeQpResidual()
{
  return _c_f[_qp]*_porosity[_qp]*_rho_w[_qp]*ImplicitBackwardDifference2::computeQpResidual();
}

Real
DarcyImplicitBackwardDifference2::computeQpJacobian()
{
  return _c_f[_qp]*_porosity[_qp]*_rho_w[_qp]*ImplicitBackwardDifference2::computeQpJacobian();
}
