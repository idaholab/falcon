#include "Material.h"
#include "HuyakornEnthalpyConvection.h"

template<>
InputParameters validParams<HuyakornEnthalpyConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

HuyakornEnthalpyConvection::HuyakornEnthalpyConvection(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

   _grad_p(coupledGradient("pressure")),
   
   _lamda(getMaterialProperty<Real>("lamda"))
   
{}

Real HuyakornEnthalpyConvection::computeQpResidual()
{
   return _lamda[_qp]* _grad_test[_i][_qp]*_grad_p[_qp];
}

Real HuyakornEnthalpyConvection::computeQpJacobian()
{
  return 0;
 
}
