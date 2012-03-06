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
#include "Water_Steam_EOS.h"
#include "Problem.h"

template<>
InputParameters validParams<CoupledTemperatureAux>()
{
    InputParameters params = validParams<AuxKernel>();

    params.addParam<Real>("temperature", 100.,"Use temperature to calculate variable density");
    
    params.addCoupledVar("pressure", "Use pressure to calculate variable density");
    params.addCoupledVar("enthalpy", "Use enthalpy ");
    params.addCoupledVar("saturation_water", "Use temperature to calculate variable density"); 
    params.addCoupledVar("density", "Use temperature to calculate mixture density");
    params.addCoupledVar("density_water","fluid density in Kg/m^3");
    params.addCoupledVar("density_steam","fluid density in Kg/m^3");
    params.addCoupledVar("enthalpy_water", "Use enthalpy water");
    params.addCoupledVar("enthalpy_steam", "Use enthalpy steam");
    params.addCoupledVar("ddensitydH_P", "Use temperature to calculate variable density");
    params.addCoupledVar("ddensitydp_H", "Use temperature to calculate variable density");
    params.addCoupledVar("denthalpy_waterdH_P", "Use temperature to calculate variable density");
    params.addCoupledVar("denthalpy_steamdH_P", "Use temperature to calculate variable density");
    params.addCoupledVar("dTdH_P", "Use temperature to calculate variable density");
    params.addCoupledVar("dswdH", "Use enthalpy calculate water saturation");
    params.addCoupledVar("denthalpy_waterdP_H", "Use temperature to calculate variable density");
    params.addCoupledVar("denthalpy_steamdP_H", "Use temperature to calculate variable density");
    params.addCoupledVar("dTdP_H", "Use temperature to calculate variable density");
    return params;
}

CoupledTemperatureAux::CoupledTemperatureAux(const std::string & name,
                                               InputParameters parameters)
    :AuxKernel(name, parameters),
    _pressure(coupledValue("pressure")),
    _enthalpy(coupledValue("enthalpy")),

    _sw(coupledValue("saturation_water")),
    _den_mix(coupledValue("density")),
    _den_water(coupledValue("density_water")),
    _den_steam(coupledValue("density_steam")),
    _enthalpy_water(coupledValue("enthalpy_water")),
    _enthalpy_steam(coupledValue("enthalpy_steam")),
    _ddendh_P(coupledValue("ddensitydH_P")),
    _ddendp_H(coupledValue("ddensitydp_H")),
    _denthalpy_waterdH_P(coupledValue("denthalpy_waterdH_P")),
    _denthalpy_steamdH_P(coupledValue("denthalpy_steamdH_P")),
    _dTdH_P(coupledValue("dTdH_P")),
    _dswdH(coupledValue("dswdH")),
    _denthalpy_waterdP_H(coupledValue("denthalpy_waterdP_H")),
    _denthalpy_steamdP_H(coupledValue("denthalpy_steamdP_H")),
    _dTdP_H(coupledValue("dTdP_H")), 
    _input_temperature(getParam<Real>("temperature"))
{}

Real
CoupledTemperatureAux::computeValue()
{
    Real _temperature = _input_temperature; 
    int _ierr;
    
    // Call EOS to get all properties
    Water_Steam_EOS::water_steam_prop_ph_(_pressure[_qp], _enthalpy[_qp], _temperature, 
                                          _sw[_qp], _den_mix[_qp],
                                          _den_water[_qp], _den_steam[_qp],
                                          _enthalpy_water[_qp], _enthalpy_steam[_qp],
                                          _ddendh_P[_qp], _ddendp_H[_qp],
                                          _denthalpy_waterdH_P[_qp],
                                          _denthalpy_steamdH_P[_qp],
                                          _dTdH_P[_qp], _dswdH[_qp],_ierr,
                                          _denthalpy_waterdP_H[_qp],
                                          _denthalpy_steamdP_H[_qp], _dTdP_H[_qp]);
        
        
     return _temperature;                           
     
 }


