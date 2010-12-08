#include "Material.h"
#include "HuyakornEnthalpyDiffusion.h"

template<>
InputParameters validParams<HuyakornEnthalpyDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

HuyakornEnthalpyDiffusion::HuyakornEnthalpyDiffusion(const std::string & name, InputParameters parameters)
  :Diffusion(name, parameters),
   
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
  return _beta[_qp]*Diffusion::computeQpJacobian();
   
}
