#include "Material.h"
#include "DarcyMassFluxZ.h"

template<>
InputParameters validParams<DarcyMassFluxZ>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

DarcyMassFluxZ::DarcyMassFluxZ(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters)
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
