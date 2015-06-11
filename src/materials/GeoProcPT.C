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

/*----------------------------------------------------------------

Contributors: Yidong Xia (INL)

Descriptions: material properties for pressure-temperature based
              T-H-M-C balance equations

----------------------------------------------------------------*/


#include "GeoProcPT.h"
#include "WaterSteamEOS.h"

template<>
InputParameters validParams<GeoProcPT>()
{
  InputParameters params = validParams<Material>();

  params.addParam<Real>(
  "permeability", 1.0e-12,
  "Intrinsic permeability [m^2], default = 1.0e-12");

  params.addParam<Real>(
  "porosity", 0.3,
  "Rock porosity, default = 0.3");

  params.addParam<Real>(
  "compressibility", 1.0e-5,
  "Total compressibility of the researvoir, default = 1.0e-5");

  params.addParam<Real>(
  "density_rock", 2.50e3,
  "Rock density [kg/m^3]");

  params.addParam<Real>(
  "gravity", 9.80665,
  "Gravity acceleration constant [m/s^2], default = 9.80665");

  params.addParam<RealGradient>(
  "gravity_direction", RealGradient(0,0,1),
  "Gravity unit directional vector, default = '0 0 1'");

  params.addParam<RealGradient>(
  "constant_pressure_gradient", RealGradient(-1,0,0),
  "Constant pressure gradient, default = '-1 0 0'");

  params.addCoupledVar(
  "pressure",
  "Assign nonlinear variable for pressure [Pa], if mass balance is involved");

  params.addCoupledVar(
  "temperature",
  "Assign nonlinear variable for temperature [K], if energy balance is involved");

  params.addParam<bool>(
  "temperature_dependent_water_properties", true,
  "Flag true if water properties are temperature dependent, default = true");

  params.addParam<bool>(
  "pressure_dependent_permeability", false,
  "Flag true if permeability is pressure dependent, default = false");

  params.addParam<Real>(
  "density_water", 1000,
  "Initial water density [kg/m^3], default = 1000");

  params.addParam<Real>(
  "viscosity_water", 0.12e-3,
  "Initial water viscosity [Pa.s], default = 0.12e-3");

  params.addParam<Real>(
  "specific_heat_rock", 0.92e3,
  "Specific heat of the rock [J/(kg.K)], default = 0.92e3");

  params.addParam<Real>(
  "specific_heat_water", 4.186e3,
  "Specific heat of water [J/(kg.K)], default = 4.186e3");

  params.addParam<Real>(
  "thermal_conductivity", 2.5,
  "Thermal conductivity of the reservoir [W/(m.K)], default = 2.5");

  return params;
}


/*******************************************************************************
Routine: GeoProcPT -- constructor
Authors: Yidong Xia
*******************************************************************************/
GeoProcPT::GeoProcPT(const std::string & name, InputParameters parameters):
  Material(name, parameters),

  // ========================================================
  // flags to indicate the involvement of terms and equations
  // ========================================================
  _has_pres(isCoupled("pressure")),
  _has_temp(isCoupled("temperature")),

  _temp_dep_weos(getParam<bool>("temperature_dependent_water_properties")),
  _pres_dep_perm(getParam<bool>("pressure_dependent_permeability")),

  // =====================
  // user-input parameters
  // =====================
  _iperm(getParam<Real>("permeability")),
  _iporo(getParam<Real>("porosity")),
  _irrho(getParam<Real>("density_rock")),
  _irsph(getParam<Real>("specific_heat_rock")),
  _icomp(getParam<Real>("compressibility")),
  _iwrho(getParam<Real>("density_water")),
  _iwvis(getParam<Real>("viscosity_water")),
  _iwsph(getParam<Real>("specific_heat_water")),
  _ithco(getParam<Real>("thermal_conductivity")),
  _igfor(getParam<Real>("gravity")),

  _igvec(getParam<RealGradient>("gravity_direction")),
  _igrdp(getParam<RealGradient>("constant_pressure_gradient")),

  // ========================
  // main nonlinear variables
  // ========================
  _pres(_has_pres ? coupledValue("pressure")    : _zero),
  _temp(_has_temp ? coupledValue("temperature") : _zero),

  _grad_pres(_has_pres ? coupledGradient("pressure") : _grad_zero),

  // ===================
  // material properties
  // ===================
  _perm(declareProperty<Real>("permeability")),
  _poro(declareProperty<Real>("porosity")),
  _rrho(declareProperty<Real>("density_rock")),
  _rsph(declareProperty<Real>("specific_heat_rock")),
  _comp(declareProperty<Real>("compressibility")),
  _wrho(declareProperty<Real>("density_water")),
  _wvis(declareProperty<Real>("viscosity_water")),
  _wtau(declareProperty<Real>("tau_water")),
  _wsph(declareProperty<Real>("specific_heat_water")),
  _thco(declareProperty<Real>("thermal_conductivity")),
  _gfor(declareProperty<Real>("gravity")),
  _drot(declareProperty<Real>("partial_rho_over_partial_temp")),

  _guvec(declareProperty<RealGradient>("gravity_vector")),
  _cgrdp(declareProperty<RealGradient>("constant_pressure_gradient")),
  _wdmfp(declareProperty<RealGradient>("darcy_mass_flux_water_pressure")),
  _wdmfe(declareProperty<RealGradient>("darcy_mass_flux_water_elevation")),
  _wdmfx(declareProperty<RealGradient>("darcy_mass_flux_water")),
  _wdflx(declareProperty<RealGradient>("darcy_flux_water"))
{}


/*******************************************************************************
Routine: computeProperties -- self-explanatory
Authors: Yidong Xia
*******************************************************************************/
void
GeoProcPT::computeProperties()
{
  for(_qp=0; _qp<_qrule->n_points(); _qp++)
    computeQpProperties();
}


/*******************************************************************************
Routine: computeQpProperties -- self-explanatory
Authors: Yidong Xia
*******************************************************************************/
void
GeoProcPT::computeQpProperties()
{
  // assign user-input values or keep default

  _perm[_qp] = _iperm;
  _poro[_qp] = _iporo;
  _rrho[_qp] = _irrho;
  _rsph[_qp] = _irsph;
  _comp[_qp] = _icomp;
  _wrho[_qp] = _iwrho;
  _wvis[_qp] = _iwvis;
  _wsph[_qp] = _iwsph;
  _thco[_qp] = _ithco;
  _gfor[_qp] = _igfor;
  _drot[_qp] = 0.0;

  _guvec[_qp] = _igvec;
  _cgrdp[_qp] = _igrdp;

  // permeability function

  if (_has_pres && _pres_dep_perm)
    mooseError("Functions for pressure-dependent permeability to be implemented");

  // water properties

  if (_has_temp && _temp_dep_weos)
  {
    _wrho[_qp] = computeTempBasedWaterDens(_temp[_qp]);
    _wvis[_qp] = computeTempBasedWaterVisc(_temp[_qp]);
    _drot[_qp] = computeTempBasedWaterPartialDensOverPartialTemp(_temp[_qp]);
  }

  // water mobility (multiplied by density)

  _wtau[_qp] = _wrho[_qp]*_perm[_qp]/_wvis[_qp];

  // compute the following vectors and use them in kernels and aux kernels

  _wdmfp[_qp] = -_wtau[_qp]*_grad_pres[_qp];
  _wdmfe[_qp] = -_wtau[_qp]*_wrho[_qp]*_gfor[_qp]*_guvec[_qp];
  _wdmfx[_qp] =  _wdmfp[_qp] + _wdmfe[_qp];
  _wdflx[_qp] =  _wdmfx[_qp]/_wrho[_qp];
}


/*******************************************************************************
Routine: computeTempBasedWaterDens
         --- compute temperature based water density
Authors: Yidong Xia
*******************************************************************************/
Real
GeoProcPT::computeTempBasedWaterDens(Real temp)
{
  return
  1e3*(1.0-pow(temp-277.1363,2)/508929.2*(temp+15.7914)/(temp-205.02037));
}


/*******************************************************************************
Routine: computeTempBasedWaterVisc
         --- compute temperature based water viscosity
Authors: Yidong Xia
*******************************************************************************/
Real
GeoProcPT::computeTempBasedWaterVisc(Real temp)
{
  if (temp>=273.15 && temp<=313.15)
    return 1.787e-3*exp((-0.03288+1.962e-4*(temp-273.15))*(temp-273.15));
  else if (temp>313.15 && temp<=373.15)
    return 0.001*pow(1.0+0.015512*(temp-293.15),-1.572);
  else if (temp>373.15 && temp<=573.15)
    return 0.2414*pow(10.0,247.8/(temp-140)-4.0);
  else
  {
    _console<<"temp = "<<temp-273.15<<" Celsius degree"<<std::endl;
    mooseError("Temperature out of [0, 300] Celsius degree \
               in temperature dependent fluid properties");
  }
  return 0.0;
}


/*******************************************************************************
Routine: computeTempBasedWaterPartialDensOverPartialTemp
         --- compute temperature based partial density over partial temperature
Authors: Yidong Xia
*******************************************************************************/
Real
GeoProcPT::computeTempBasedWaterPartialDensOverPartialTemp(Real temp)
{
  return
  -1e3*(
          2.0*(temp-277.1363)/508929.2*(temp+15.7914)/(temp-205.02037)
        + pow(temp-277.1363,2)/508929.2
          *(1.0/(temp-205.02037)-(temp+15.7914)/pow((temp-205.02037),2))
       );
}
