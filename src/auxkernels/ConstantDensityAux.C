#include "ConstantDensityAux.h"

template<>
InputParameters validParams<ConstantDensityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("density_water", 1000.,"fluid density in Kg/m^3");
  return params;
}

ConstantDensityAux::ConstantDensityAux(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters),
   _input_density_water(getParam<Real>("density_water"))

{}

Real
ConstantDensityAux::computeValue()
{
  return _input_density_water;
}


