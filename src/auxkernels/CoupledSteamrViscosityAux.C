/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "CoupledSteamViscosityAux.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledSteamViscosityAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("density_steam", "Use temperature to calculate variable viscosity");
    params.addCoupledVar("temperature", "Use temperature to calculate variable viscosity");
    params.addParam<bool>("temp_dependent_viscosity", true, "Flag to call viscosity routine");
    params.addParam<Real>("viscosity_steam", 1E-5,"fluid viscosity in Pa sec");
    return params;
}

CoupledSteamViscosityAux::CoupledSteamViscosityAux(const std::string & name, InputParameters parameters)
:AuxKernel(name, parameters),
_density_steam(coupledValue("density_steam")),
_temperature(coupledValue("temperature")),
_input_viscosity_steam(getParam<Real>("viscosity_steam")),
_has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))

{}


Real
CoupledSteamViscosityAux::computeValue()
{
    
    if  (_has_variable_viscosity == true) //then call the viscosity subroutine
    {
        
        Real _viscosity_subroutine_val = 1E-5;
        
        Water_Steam_EOS::viss_noderiv1_( _density_steam[_qp], _temperature[_qp], _viscosity_subroutine_val);
        return _viscosity_subroutine_val; 
    }
    
    else //just use default water viscosity or values from input
    {
        return _input_viscosity_steam;
    }
}
