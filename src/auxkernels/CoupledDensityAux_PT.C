#include "CoupledDensityAux_PT.h"
#include "Water_Steam_EOS.h"


template<>
InputParameters validParams<CoupledDensityAux_PT>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addCoupledVar("temperature", "Use temperature to calculate variable density");
  params.addCoupledVar("pressure", "Use pressure to calculate variable density");
  params.addCoupledVar("dwdt", "Use pressure to calculate variable density");
  params.addCoupledVar("dwdp", "Use pressure to calculate variable density");  
  params.addParam<bool>("temp_dependent_density", true, "Flag to call density and viscosity routine");
  params.addParam<Real>("density_water", 1000.,"fluid density in Kg/m^3");
  return params;
}

CoupledDensityAux_PT::CoupledDensityAux_PT(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters),
   _temperature(coupledValue("temperature")),
   _pressure(coupledValue("pressure")),
   _dwdt(coupledValue("dwdt")),
   _dwdp(coupledValue("dwdp")),
   _input_density_water(getParam<Real>("density_water")),
   _has_variable_density(getParam<bool>("temp_dependent_density"))
{}

Real
CoupledDensityAux_PT::computeValue()
{
  if  (_has_variable_density == true) //then call the density and viscosity functions
    {
      //Function call to "density_fun" to calc density_water using the coupled temperature value
      Real _density_subroutine_val = 1000;
      // EOS for pure liquid water (0-373C; 0-1000MPa)
//      _temperature[_qp] =200.0;
      
      Water_Steam_EOS::wateos_noderiv1_( _temperature[_qp], _pressure[_qp], _density_subroutine_val);

       Real _density1 = _density_subroutine_val;
       Real _density2 = 1.0;
       Real _tmp1;
       Real _tmp0;
        
        //      _temperature[_qp] =200.0;
        
        _tmp0 = _pressure[_qp];
        _tmp1 = _tmp0 + 1E-1;
        
        Water_Steam_EOS::wateos_noderiv1_( _temperature[_qp], _tmp1, _density2);
        
        _dwdp[_qp]= (_density2-_density1)/1E-1;

        _tmp0 = _temperature[_qp];
        _tmp1 = _tmp0 + 1E-6;
        
        Water_Steam_EOS::wateos_noderiv1_( _tmp1, _pressure[_qp], _density2);
        
        _dwdt[_qp]= (_density2-_density1)/1E-6;    

   //    std::cout <<"ddd:"<< _dwdt[_qp]<< "\n";
      return _density1;                           
      // std::cout <<_density__subroutine_val<< "\n";
    }

   else //just use default water density from input  values
    { 
        _dwdp[_qp] = 0.;
        _dwdt[_qp] = 0.;
      return _input_density_water;
    }
}



//Function to calc water density, single phase conditions only
//Real
//CoupledDensityAux::density_fun(Real T)
//{
//return 1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
//}
//end density function
