#include "CoupledAux.h"

template<>
InputParameters validParams<CoupledAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("coupled", "Coupled Value");
  
  params.set<Real>("value")=0.0;
  return params;
}

CoupledAux::CoupledAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _coupled(coupled("coupled")),
   _coupled_val(coupledValue("coupled")),
   _value(getParam<Real>("value"))
{}


Real
CoupledAux::computeValue()
{
  return _coupled_val[_qp]+_value;
}
