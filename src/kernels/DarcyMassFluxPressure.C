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
   _darcy_params_w(getMaterialProperty<Real>("darcy_params_w"))
{}

Real
DarcyMassFluxPressure::computeQpResidual()
{
 
  return _darcy_params_w[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressure::computeQpJacobian()
{
  return _darcy_params_w[_qp]*Diffusion::computeQpJacobian();
}
