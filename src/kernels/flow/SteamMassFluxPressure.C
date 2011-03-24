#include "Material.h"
#include "SteamMassFluxPressure.h"

template<>
InputParameters validParams<SteamMassFluxPressure>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

SteamMassFluxPressure::SteamMassFluxPressure(const std::string & name,
                                             InputParameters parameters)
  :Diffusion(name, parameters),
   _tau_steam(getMaterialProperty<Real>("tau_steam"))
{}

Real
SteamMassFluxPressure::computeQpResidual()
{
  
  return _tau_steam[_qp]*Diffusion::computeQpResidual();
}

Real
SteamMassFluxPressure::computeQpJacobian()
{
  return _tau_steam[_qp]*Diffusion::computeQpJacobian();
}
