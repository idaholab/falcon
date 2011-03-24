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
   _tau_steam(getMaterialProperty<Real>("tau_steam"))
{}

Real
DarcyMassFluxPressureSteam::computeQpResidual()
{
  
  return _tau_steam[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressureSteam::computeQpJacobian()
{
  return _tau_steam[_qp]*Diffusion::computeQpJacobian();
}
