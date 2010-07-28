#include "TemperatureAux.h"

template<>
InputParameters validParams<TemperatureAux>()
{
     InputParameters params = validParams<AuxKernel>();
     return params;
}

TemperatureAux::TemperatureAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
     _temp(getMaterialProperty<Real>("temperature"))
   
{}


Real
TemperatureAux::computeValue()
{
      return _temp[_qp];

}
