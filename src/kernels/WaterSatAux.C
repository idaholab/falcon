#include "WaterSatAux.h"

template<>
InputParameters validParams<WaterSatAux>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

WaterSatAux::WaterSatAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
   _sat_w(getMaterialProperty<Real>("sat_w"))

{}

Real
WaterSatAux::computeValue()
{
             
 return  _sat_w[_qp];

}
