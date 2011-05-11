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
  //std::cout << _u [_qp]<<' '<< _grad_u [_qp]<< "\n";
  return _thermal_conductivity[_qp]*Diffusion::computeQpResidual()*_dt;

}

Real
TemperatureDiffusion::computeQpJacobian()
{
    return _thermal_conductivity[_qp]*Diffusion::computeQpJacobian()*_dt;
}



