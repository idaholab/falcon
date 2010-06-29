#include "Material.h"
#include "DarcyMassFluxPressure.h"

template<>
InputParameters validParams<DarcyMassFluxPressure>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

DarcyMassFluxPressure::DarcyMassFluxPressure(std::string name,
                                             MooseSystem & moose_system,
                                             InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
   _darcy_params_w(getRealMaterialProperty("darcy_params_w"))
{}

Real
DarcyMassFluxPressure::computeQpResidual()
{
  //Real residue2;
  //   residue2 = _darcy_params_w[_qp]*Diffusion::computeQpResidual();

  //   std::cout << "residue2" << residue2 <<" .\n";
  return _darcy_params_w[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressure::computeQpJacobian()
{
  return _darcy_params_w[_qp]*Diffusion::computeQpJacobian();
}
