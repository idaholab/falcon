#include "Material.h"
#include "DarcyMassFluxPressureSteam.h"

template<>
InputParameters validParams<DarcyMassFluxPressureSteam>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

DarcyMassFluxPressureSteam::DarcyMassFluxPressureSteam(std::string name,
                                             MooseSystem & moose_system,
                                             InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
   _darcy_params_s(getRealMaterialProperty("darcy_params_s"))
{}

Real
DarcyMassFluxPressureSteam::computeQpResidual()
{
  //Real residue3;
  //   residue3 = _darcy_params_s[_qp]*Diffusion::computeQpResidual();

  //   std::cout << "residue3" << residue3 << " .\n";
  return _darcy_params_s[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressureSteam::computeQpJacobian()
{
  return _darcy_params_s[_qp]*Diffusion::computeQpJacobian();
}
