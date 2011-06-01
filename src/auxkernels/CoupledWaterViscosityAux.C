#include "CoupledWaterViscosityAux.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledWaterViscosityAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("temperature", "Use temperature to calculate variable viscosity");
    params.addCoupledVar("pressure", "Use pressure to calculate variable viscosity");
    params.addParam<bool>("temp_dependent_viscosity", true, "Flag to call viscosity routine");
    params.addParam<Real>("viscosity_water", 0.001,"fluid viscosity in Pa.s");
    return params;
}

CoupledWaterViscosityAux::CoupledWaterViscosityAux(const std::string & name, InputParameters parameters)
 :AuxKernel(name, parameters),
  _temperature(coupledValue("temperature")),
  _pressure(coupledValue("pressure")),
  _input_viscosity_water(getParam<Real>("viscosity_water")),
  _has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))
{}


Real
CoupledWaterViscosityAux::computeValue()
{
  if  (_has_variable_viscosity == true) //then call the viscosity subroutine
  {
    Real _viscosity_subroutine_val = 0.001;
    Water_Steam_EOS::visw_noderiv1_( _temperature[_qp], _pressure[_qp], _viscosity_subroutine_val);
    return _viscosity_subroutine_val; 
  }
  else //just use default water viscosity or values from input
  {
    return _input_viscosity_water;
  }
}
