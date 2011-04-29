#include "CoupledViscosityAux.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledViscosityAux>()
{
  InputParameters params = validParams<AuxKernel>();
 params.addCoupledVar("temperature", "Use temperature to calculate variable viscosity");
  params.addCoupledVar("pressure", "Use pressure to calculate variable viscosity");
  
  params.addParam<bool>("temp_dependent_viscosity", true, "Flag to call viscosity routine");
  params.addParam<Real>("viscosity_water", 9.999e-3,"fluid viscosity in Pa sec");
  return params;
}

CoupledViscosityAux::CoupledViscosityAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
    _temperature(coupledValue("temperature")),
     _pressure(coupledValue("pressure")),
   
   _input_viscosity_water(getParam<Real>("viscosity_water")),
   _has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))

{}


Real
CoupledViscosityAux::computeValue()
{

  if  (_has_variable_viscosity == true) //then call the viscosity subroutine
    {

      Real _viscosity_subroutine_val = 1000;
   
      
      Water_Steam_EOS::visw_noderiv1_( _temperature[_qp], _pressure[_qp], _viscosity_subroutine_val);
      return _viscosity_subroutine_val; 
      
      //Function call to "density_fun" to calc density_water using the coupled temperature value
      //return viscosity_fun((_temperature)[_qp]);
    }

   else //just use default water viscosity or values from input
    {
      return _input_viscosity_water;
    }
}

/*

//Function call to calc viscosity
Real
CoupledViscosityAux::viscosity_fun(Real T)
{
  Real _viscosity_water, a, b, c;
  
  if (T < 0.)
    {
      std::cerr << "T= " << T ;
      mooseError("Temperature out of Range");
    }
    
    else if (T <= 40.)
    {
      a = 1.787E-3;
      b = (-0.03288+(1.962E-4*T))*T;
      _viscosity_water = a * exp(b);
     }
    
    else if (T <= 100.)
    {
      a = 1e-3;
      b = (1+(0.015512*(T-20)));
      c = -1.572;
      _viscosity_water = a * pow(b,c);
    }
    
    else if (T <= 300.)
    {
      a = 0.2414;
      b = 247 / (T+133.15);
      c = (a * pow(10,b));
      _viscosity_water = c * 1E-4;
     }
    
    else
    {
      std::cerr << "T= " << T;
      mooseError("Temperature out of Range");
    }
  return (_viscosity_water);
}
//end viscosity function
*/
