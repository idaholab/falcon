#include "Material.h"
#include "TemperatureDiffusion.h"

template<>
InputParameters validParams<TemperatureDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

TemperatureDiffusion::TemperatureDiffusion(const std::string & name, InputParameters parameters)
  :Diffusion(name, parameters),
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
TemperatureDiffusion::computeQpResidual()
{
  
  return _thermal_conductivity[_qp]*Diffusion::computeQpResidual();
//  std::cout << _u << "\n";
}

Real
TemperatureDiffusion::computeQpJacobian()
{
  return _thermal_conductivity[_qp]*Diffusion::computeQpJacobian();
}



