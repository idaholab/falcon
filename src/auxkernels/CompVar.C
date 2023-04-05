/*
AuxKernel of Passing Variable
*/

#include "CompVar.h"

registerMooseObject("FalconApp", CompVar);

InputParameters
CompVar::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addRequiredCoupledVar("coupled","Nonlinear Variable that needed to be passed");

  return params;
}

CompVar::CompVar(const InputParameters & parameters)
  : AuxKernel(parameters),
  
  _coupled_val(coupledValue("coupled"))

{
}

Real
CompVar::computeValue()
{
  return _coupled_val[_qp];
}