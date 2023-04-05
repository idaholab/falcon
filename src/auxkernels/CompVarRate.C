/*
AuxKernel of Passing Variable Time Derivative 
*/

#include "CompVarRate.h"

registerMooseObject("FalconApp", CompVarRate);

InputParameters
CompVarRate::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addRequiredCoupledVar("coupled","Nonlinear Variable that needed to be taken time derivative of");

  return params;
}

CompVarRate::CompVarRate(const InputParameters & parameters)
  : AuxKernel(parameters),
  
  //Compute the time derivative of the given variable using "coupledDot"
  _coupled_val(coupledDot("coupled"))

{
}

Real
CompVarRate::computeValue()
{
  return _coupled_val[_qp];
}