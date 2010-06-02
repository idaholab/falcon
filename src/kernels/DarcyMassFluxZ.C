#include "Material.h"
#include "DarcyMassFluxZ.h"

template<>
InputParameters validParams<DarcyMassFluxZ>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

DarcyMassFluxZ::DarcyMassFluxZ(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _darcy_params(getRealMaterialProperty("darcy_params")),
   _gravity(getRealMaterialProperty("gravity")),
   _rho_w(getRealMaterialProperty("rho_w")),
   _gravity_vector(getRealVectorValueMaterialProperty("gravity_vector"))
{}

Real
DarcyMassFluxZ::computeQpResidual()
{
  return _darcy_params[_qp]*_gravity[_qp]*_rho_w[_qp]*_gravity_vector[_qp]*_dphi[_i][_qp];
}

Real
DarcyMassFluxZ::computeQpJacobian()
{
return 0;
}
