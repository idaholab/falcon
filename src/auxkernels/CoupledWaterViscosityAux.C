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

#include "CoupledWaterViscosityAux.h"
#include "WaterSteamEOS.h"

template<>
InputParameters validParams<CoupledWaterViscosityAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("density_water", "Use temperature to calculate variable viscosity");
    params.addCoupledVar("temperature", "Use temperature to calculate variable viscosity");
    params.addParam<bool>("temp_dependent_viscosity", true, "Flag to call viscosity routine");
    params.addParam<Real>("viscosity_water", 0.001,"fluid viscosity in Pa.s");
    params.addRequiredParam<UserObjectName>("water_steam_properties", "New C++ Code for fluid properties");

    return params;
}

CoupledWaterViscosityAux::CoupledWaterViscosityAux(const std::string & name, InputParameters parameters)
 :AuxKernel(name, parameters),
_water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),
  _density_water(coupledValue("density_water")),
  _temperature(coupledValue("temperature")),
  _input_viscosity_water(getParam<Real>("viscosity_water")),
  _has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))
{}


Real
CoupledWaterViscosityAux::computeValue()
{
  if  (_has_variable_viscosity == true) //then call the viscosity subroutine
  {
    Real _viscosity_subroutine_val = 0.001;
 //   Water_Steam_EOS::visw_noderiv1_( _temperature[_qp], _pressure[_qp], _viscosity_subroutine_val);
      
      _water_steam_properties.viscosity( _density_water[_qp], _temperature[_qp], _viscosity_subroutine_val);
 
    //Water_Steam_EOS::viss_noderiv1_( _density_water[_qp], _temperature[_qp], _viscosity_subroutine_val);     
      
    return _viscosity_subroutine_val; 
  }
  else //just use default water viscosity or values from input
  {
    return _input_viscosity_water;
  }
}
