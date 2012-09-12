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

#include "CoupledTemperatureAux.h"
#include "WaterSteamEOS.h"

template<>
InputParameters validParams<CoupledTemperatureAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("pressure", "Use pressure to calculate variable temperature");
    params.addCoupledVar("enthalpy", "Use enthalpy to calculate variable temperature");
    params.addParam("temperature", 100., "Initial value for temperature");
    params.addRequiredParam<UserObjectName>("water_steam_properties", "EOS functions, calculate water and steam properties");
    return params;
}

CoupledTemperatureAux::CoupledTemperatureAux(const std::string & name,
                                               InputParameters parameters)
    :AuxKernel(name, parameters),
    _water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),

    _pressure(coupledValue("pressure")),
    _enthalpy(coupledValue("enthalpy")),
    _input_temperature(getParam<Real>("temperature"))
{}

Real
CoupledTemperatureAux::computeValue()
{
    Real _temperature = _input_temperature;
    Real _dummy_var[12];
    
    _water_steam_properties.waterAndSteamEquationOfStatePropertiesPH (_enthalpy[_qp], _pressure[_qp], _dummy_var[0] , _temperature, _dummy_var[1], _dummy_var[2], _dummy_var[3], _dummy_var[4], _dummy_var[5], _dummy_var[6], _dummy_var[7], _dummy_var[8], _dummy_var[9], _dummy_var[10], _dummy_var[11]);
    
    return _temperature;

}

