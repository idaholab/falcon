#include "VelocityAux.h"

template<>
InputParameters validParams<VelocityAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.set<int>("component")= 0;
     return params;
}

VelocityAux::VelocityAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
    _i(parameters.get<int>("component")),
    _pore_velocity_w(getMaterialProperty<RealGradient>("pore_velocity_w"))   
{}


Real
VelocityAux::computeValue()
{
  return _pore_velocity_w[_qp](_i);

}
