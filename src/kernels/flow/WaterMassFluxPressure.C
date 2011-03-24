#include "Material.h"
#include "WaterMassFluxPressure.h"

template<>
InputParameters validParams<WaterMassFluxPressure>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

WaterMassFluxPressure::WaterMassFluxPressure(const std::string & name,
                                             InputParameters parameters)
  :Diffusion(name, parameters),
   _tau_water(getMaterialProperty<Real>("tau_water"))
{}

Real
WaterMassFluxPressure::computeQpResidual()
{
 
  return _tau_water[_qp]*Diffusion::computeQpResidual();
}

Real
WaterMassFluxPressure::computeQpJacobian()
{
  return _tau_water[_qp]*Diffusion::computeQpJacobian();
}

