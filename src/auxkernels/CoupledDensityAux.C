#include "CoupledDensityAux.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledDensityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("temperature", "Use temperature to calculate variable density");
  params.addRequiredCoupledVar("pressure", "Use pressure to calculate variable density");
  
  params.addParam<bool>("temp_dependent_density", true, "Flag to call density and viscosity routine");
  params.addParam<Real>("density_water", 999.9,"fluid density in Kg/m^3");
  return params;
}

CoupledDensityAux::CoupledDensityAux(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters),
   _temperature(coupledValue("temperature")),
   _pressure(coupledValue("pressure")),
   
   _input_density_water(getParam<Real>("density_water")),
   _has_variable_density(getParam<bool>("temp_dependent_density"))


{}


Real
CoupledDensityAux::computeValue()
{
  if  (_has_variable_density == true) //then call the density and viscosity functions
    {
      //Function call to "density_fun" to calc density_water using the coupled temperature value
      _density_subroutine_val = 0;
      
      Water_Steam_EOS::watereos1_(_pressure[_qp], _temperature[_qp], _density_subroutine_val);
      return _density_subroutine_val;                           

        //return density_fun((_temperature)[_qp]);
      
    }

   else //just use default water density from input  values
    {
      return _input_density_water;
    }
}



//Function to calc water density, single phase conditions only
Real
CoupledDensityAux::density_fun(Real T)
{
return 1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
}
//end density function
