#include "VelocityAux.h"

template<>
InputParameters validParams<VelocityAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.set<int>("component")= 0;
     return params;
}

VelocityAux::VelocityAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
    _i(getParam<int>("component")),
    _pore_velocity_w(getMaterialProperty<RealGradient>("pore_velocity_w"))   
{}


Real
VelocityAux::computeValue()
{
  return _pore_velocity_w[_qp](_i);

}
