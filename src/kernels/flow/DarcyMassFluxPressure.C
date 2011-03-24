#include "Material.h"
#include "DarcyMassFluxPressure.h"

template<>
InputParameters validParams<DarcyMassFluxPressure>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

DarcyMassFluxPressure::DarcyMassFluxPressure(const std::string & name,
                                             InputParameters parameters)
  :Diffusion(name, parameters),
   _tau_water(getMaterialProperty<Real>("tau_water"))
{}

Real
DarcyMassFluxPressure::computeQpResidual()
{
 
  return _tau_water[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressure::computeQpJacobian()
{
  return _tau_water[_qp]*Diffusion::computeQpJacobian();
}
