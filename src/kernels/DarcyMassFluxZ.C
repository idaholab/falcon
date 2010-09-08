#include "Material.h"
#include "DarcyMassFluxZ.h"

template<>
InputParameters validParams<DarcyMassFluxZ>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

DarcyMassFluxZ::DarcyMassFluxZ(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _darcy_params_w(getMaterialProperty<Real>("darcy_params_w")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _rho_w(getMaterialProperty<Real>("rho_w")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
DarcyMassFluxZ::computeQpResidual()
{
  return _darcy_params_w[_qp]*_gravity[_qp]*_rho_w[_qp]*_gravity_vector[_qp]*_grad_phi[_i][_qp];
}

Real
DarcyMassFluxZ::computeQpJacobian()
{
return 0;
}
