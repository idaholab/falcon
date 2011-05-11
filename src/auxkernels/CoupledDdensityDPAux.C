#include "CoupledDdensityDPAux.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledDdensityDPAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("temperature", "Use temperature to calculate variable density");
    params.addCoupledVar("pressure", "Use pressure to calculate variable density");
    //  params.addParam<bool>("temp_dependent_density", true, "Flag to call density and viscosity routine");
    //  params.addParam<Real>("density_water", 1000.,"fluid density in Kg/m^3");
    return params;
}

CoupledDdensityDPAux::CoupledDdensityDPAux(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters),
_temperature(coupledValue("temperature")),
_pressure(coupledValue("pressure"))
//   _input_density_water(getParam<Real>("density_water")),
//   _has_variable_density(getParam<bool>("temp_dependent_density"))
{}

Real
CoupledDdensityDPAux::computeValue()
{
    
    Real _ddensity_dp = 0.0;
    Real _density1 = 1.0;
    Real _density2 = 1.0;
    Real _tmp1;
    Real _tmp0;
    
    //      _temperature[_qp] =200.0;
    
    _tmp0 = _pressure[_qp];
    _tmp1 = _tmp0 + 1E-1;
    
    Water_Steam_EOS::wateos_noderiv1_( _temperature[_qp], _tmp0, _density1);
    Water_Steam_EOS::wateos_noderiv1_( _temperature[_qp], _tmp1, _density2);
    
    return (_density2-_density1)/1E-1;
    
    // std::cout <<_density__subroutine_val<< "\n";
    
    
}



//Function to calc water density, single phase conditions only
//Real
//CoupledDdensityDPAux::density_fun(Real T)
//{
//return 1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
//}
//end density function
