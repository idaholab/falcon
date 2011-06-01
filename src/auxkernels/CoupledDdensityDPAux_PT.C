#include "CoupledDdensityDPAux_PT.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledDdensityDPAux_PT>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("temperature", "Use temperature to calculate variable density");
    params.addCoupledVar("pressure", "Use pressure to calculate variable density");
    //  params.addParam<bool>("temp_dependent_density", true, "Flag to call density and viscosity routine");
    //  params.addParam<Real>("density_water", 1000.,"fluid density in Kg/m^3");
    return params;
}

CoupledDdensityDPAux_PT::CoupledDdensityDPAux_PT(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters),
_temperature(coupledValue("temperature")),
_pressure(coupledValue("pressure"))
//   _input_density_water(getParam<Real>("density_water")),
//   _has_variable_density(getParam<bool>("temp_dependent_density"))
{}

Real
CoupledDdensityDPAux_PT::computeValue()
{
    
     
    return _u[_qp];
    
    // std::cout <<_density__subroutine_val<< "\n";
    
    
}



//Function to calc water density, single phase conditions only
//Real
//CoupledDdensityDPAux::density_fun(Real T)
//{
//return 1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
//}
//end density function
