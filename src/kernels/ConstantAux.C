#include "ConstantAux.h"

template<>
InputParameters validParams<ConstantAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.set<Real>("value")=0.0;
  return params;
}

ConstantAux::ConstantAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _value(getParam<Real>("value"))
{}


Real
ConstantAux::computeValue()
{
  return _value;
}
