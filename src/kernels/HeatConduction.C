#include "HeatConduction.h"

template<>
InputParameters validParams<HeatConduction>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

HeatConduction::HeatConduction(const std::string & name, InputParameters parameters)
  :Diffusion(name, parameters),
   _k(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
HeatConduction::computeQpResidual()
{
  return _k[_qp]*Diffusion::computeQpResidual();
}

Real
HeatConduction::computeQpJacobian()
{
  return _k[_qp]*Diffusion::computeQpJacobian();
}
