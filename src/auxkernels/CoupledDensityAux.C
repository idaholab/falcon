#include "CoupledDensityAux.h"

template<>
InputParameters validParams<CoupledDensityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.set<Real>("value")=0.0;
  return params;
}

CoupledDensityAux::CoupledDensityAux(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
   _temperature(coupled("temperature")),
   _temperature_val(coupledValue("temperature")),
   _value(getParam<Real>("value"))
{}


Real
CoupledDensityAux::computeValue()
{
  return 1000.*(1-((pow(((_temperature_val[_qp])-3.9863),2)/508929.2)*(((_temperature_val[_qp])+288.9414)/((_temperature_val[_qp])+68.12963))));
}
