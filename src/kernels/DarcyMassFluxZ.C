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
   _darcy_params_w(getRealMaterialProperty("darcy_params_w")),
   _gravity(getRealMaterialProperty("gravity")),
   _rho_w(getRealMaterialProperty("rho_w")),
   _gravity_vector(getRealVectorValueMaterialProperty("gravity_vector"))
{}

Real
DarcyMassFluxZ::computeQpResidual()
{

  // Real residue4;
  //   residue4 = _darcy_params_w[_qp]*_gravity[_qp]*_rho_w[_qp]*_gravity_vector[_qp]*_grad_phi[_i][_qp];

  //    std::cout << "residue4" << residue4 << " .\n";
  return _darcy_params_w[_qp]*_gravity[_qp]*_rho_w[_qp]*_gravity_vector[_qp]*_grad_phi[_i][_qp];

  
}

Real
DarcyMassFluxZ::computeQpJacobian()
{
return 0;
}
