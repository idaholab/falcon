#include "Material.h"
#include "EnthalpyDiffusion.h"

template<>
InputParameters validParams<EnthalpyDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  params.addCoupledVar("tempAux", "TODO: add description");
  return params;
}

EnthalpyDiffusion::EnthalpyDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Diffusion(name, moose_system, parameters),

   _grad_T( coupledGradient("tempAux")),
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
EnthalpyDiffusion::computeQpResidual()
{
  return _thermal_conductivity[_qp]*(_grad_T[_qp]*_grad_test[_i][_qp]);
  
}

Real
EnthalpyDiffusion::computeQpJacobian()
{
  return 0;

}
