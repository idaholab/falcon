#include "CoupledPorosityAux.h"

template<>
InputParameters validParams<CoupledPorosityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("porosity", 0.3,"Initial reservoir porosity");
  return params;
}

CoupledPorosityAux::CoupledPorosityAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   //  _temperature(coupledValue("temperature")),
   _input_porosity(getParam<Real>("porosity"))
//   _has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))

{}


Real
CoupledPorosityAux::computeValue()
{
return _input_porosity;
}


