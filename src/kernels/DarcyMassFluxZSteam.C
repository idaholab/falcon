#include "Material.h"
#include "DarcyMassFluxZSteam.h"

template<>
InputParameters validParams<DarcyMassFluxZSteam>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

DarcyMassFluxZSteam::DarcyMassFluxZSteam(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _darcy_params_s(getRealMaterialProperty("darcy_params_s")),
   _gravity(getRealMaterialProperty("gravity")),
   _rho_s(getRealMaterialProperty("rho_s")),
   _gravity_vector(getRealVectorValueMaterialProperty("gravity_vector"))
{}

Real
DarcyMassFluxZSteam::computeQpResidual()
{
  //Real residue5;
  //   residue5 =_darcy_params_s[_qp]*_gravity[_qp]*_rho_s[_qp]*_gravity_vector[_qp]*_grad_test[_i][_qp];

  // std::cout << "residue5" << residue5 << " .\n";
 
  return _darcy_params_s[_qp]*_gravity[_qp]*_rho_s[_qp]*_gravity_vector[_qp]*_grad_test[_i][_qp];
}

Real
DarcyMassFluxZSteam::computeQpJacobian()
{
return 0;
}
