#include "CoupledAux.h"

template<>
InputParameters validParams<CoupledAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("coupled", "Coupled Value");
  
  params.set<Real>("value")=0.0;
  return params;
}

CoupledAux::CoupledAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
   _coupled(coupled("coupled")),
   _coupled_val(coupledValue("coupled")),
   _value(_parameters.get<Real>("value"))
{}


Real
CoupledAux::computeValue()
{
  return _coupled_val[_qp]+_value;
}
