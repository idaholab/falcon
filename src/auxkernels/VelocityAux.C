#include "VelocityAux.h"

template<>
InputParameters validParams<VelocityAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.addParam<int>("component",0,"component of the pressure vector");
     return params;
}

VelocityAux::VelocityAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _darcy_flux_water(getMaterialProperty<RealGradient>("darcy_flux_water")),
   _i(getParam<int>("component"))

{}

Real
VelocityAux::computeValue()
{
  return _darcy_flux_water[_qp](_i);
  

}
