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
   _darcy_params(getRealMaterialProperty("darcy_params"))
{}

Real
DarcyMassFluxPressure::computeQpResidual()
{
  return _darcy_params[_qp]*Diffusion::computeQpResidual();
}

Real
DarcyMassFluxPressure::computeQpJacobian()
{
  return _darcy_params[_qp]*Diffusion::computeQpJacobian();
}
