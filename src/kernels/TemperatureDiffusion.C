#include "Material.h"
#include "TemperatureDiffusion.h"

template<>
InputParameters validParams<TemperatureDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

TemperatureDiffusion::TemperatureDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
TemperatureDiffusion::computeQpResidual()
{
  return _thermal_conductivity[_qp]*Diffusion::computeQpResidual();
}

Real
TemperatureDiffusion::computeQpJacobian()
{
  return _thermal_conductivity[_qp]*Diffusion::computeQpJacobian();
}
