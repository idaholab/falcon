#include "Material.h"
#include "HuyakornEnthalpyDiffusion.h"

template<>
InputParameters validParams<HuyakornEnthalpyDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

HuyakornEnthalpyDiffusion::HuyakornEnthalpyDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
   
   _beta(getMaterialProperty<Real>("beta"))
   
{}

Real
HuyakornEnthalpyDiffusion::computeQpResidual()
{
  return _beta[_qp]*Diffusion::computeQpResidual();
}

Real
HuyakornEnthalpyDiffusion::computeQpJacobian()
{
  return 0;
   
}
