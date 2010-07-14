#include "Material.h"
#include "DarcyMassFluxPressure.h"

template<>
InputParameters validParams<DarcyMassFluxPressure>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

DarcyMassFluxPressure::DarcyMassFluxPressure(std::string name,
                                             MooseSystem & moose_system,
                                             InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
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
