#include "Material.h"
#include "DarcyMassFluxPressureSteam.h"

template<>
InputParameters validParams<DarcyMassFluxPressureSteam>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

DarcyMassFluxPressureSteam::DarcyMassFluxPressureSteam(const std::string & name,
                                             InputParameters parameters)
  :Diffusion(name, parameters),
   _darcy_params_s(getMaterialProperty<Real>("darcy_params_s"))
{}

Real
DarcyMassFluxPressureSteam::computeQpResidual()
{
  
  return _darcy_params_s[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressureSteam::computeQpJacobian()
{
  return _darcy_params_s[_qp]*Diffusion::computeQpJacobian();
}
