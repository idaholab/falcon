#include "ConstantViscosityAux.h"

template<>
InputParameters validParams<ConstantViscosityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("viscosity_water", 1000.,"fluid density in Kg/m^3");
  return params;
}

ConstantViscosityAux::ConstantViscosityAux(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters),
   _input_viscosity_water(getParam<Real>("viscosity_water"))

{}

Real
ConstantViscosityAux::computeValue()
{
  return _input_viscosity_water;
}


