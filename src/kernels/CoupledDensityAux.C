#include "CoupledDensityAux.h"

template<>
InputParameters validParams<CoupledDensityAux>()
{
  InputParameters params;
  params.set<Real>("value")=0.0;
  return params;
}

CoupledDensityAux::CoupledDensityAux(std::string name,
                         InputParameters parameters,
                         std::string var_name,
                         std::vector<std::string> coupled_to,
                         std::vector<std::string> coupled_as)
  :AuxKernel(name, parameters, var_name, coupled_to, coupled_as),
   _temperature(coupled("temperature")),
   _temperature_val(coupledValAux("temperature")),
   _value(_parameters.get<Real>("value"))
{}


Real
CoupledDensityAux::computeValue()
{
  return 1000.*(1-((pow(((_temperature_val)-3.9863),2)/508929.2)*(((_temperature_val)+288.9414)/((_temperature_val)+68.12963))));
}
