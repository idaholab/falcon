#include "Material.h"
#include "HuyakornEnthalpyDiffusion.h"

template<>
InputParameters validParams<HuyakornEnthalpyDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("tempAux", "TODO: add description");
  return params;
}

HuyakornEnthalpyDiffusion::HuyakornEnthalpyDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
   
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity")),
   _dTbydH_P(getMaterialProperty<Real>("dTbydH_P"))
   
{}

Real
HuyakornEnthalpyDiffusion::computeQpResidual()
{
  Real beta;
  
  beta = _thermal_conductivity[_qp]*_dTbydH_P[_qp];

  return beta*Diffusion::computeQpResidual();
}

Real
HuyakornEnthalpyDiffusion::computeQpJacobian()
{
  return 0;
   
}
