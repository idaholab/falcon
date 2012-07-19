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

#include "FluidFlow.h"
#include "Banana_Slug.h"

template<>
InputParameters validParams<FluidFlow>()
{
  InputParameters params = validParams<PorousMedia>(); 
  params.addCoupledVar("pressure", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("enthalpy", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
  params.addRequiredParam<std::string>("water_steam_properties", "user data object being used");
  return params;
}

FluidFlow::FluidFlow(const std::string & name, InputParameters parameters) :
    PorousMedia (name, parameters),
    _water_steam_properties(getUserObject<Banana_Slug>("water_steam_properties")),
    
    _has_pressure(isCoupled("pressure")),
    _grad_p(_has_pressure ? coupledGradient("pressure") : _grad_zero),
    _pressure(_has_pressure ? coupledValue("pressure")  : _zero),
    
    _has_enthalpy(isCoupled("enthalpy")), 
    _enthalpy(_has_enthalpy ? coupledValue("enthalpy")  : _zero),

    _tau_water(declareProperty<Real>("tau_water")),
    _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
    _darcy_mass_flux_water(declareProperty<RealGradient>("darcy_mass_flux_water")),
    _darcy_mass_flux_water_pressure(declareProperty<RealGradient>("darcy_mass_flux_water_pressure")),
    _darcy_mass_flux_water_elevation(declareProperty<RealGradient>("darcy_mass_flux_water_elevation")),
    _Dtau_waterDP(declareProperty<Real>("Dtau_waterDP")),
    _Dtau_waterDH(declareProperty<Real>("Dtau_waterDH")),

    _tau_steam(declareProperty<Real>("tau_steam")),
    _darcy_flux_steam(declareProperty<RealGradient>("darcy_flux_steam")),
    _darcy_mass_flux_steam(declareProperty<RealGradient>("darcy_mass_flux_steam")),
    _darcy_mass_flux_steam_pressure(declareProperty<RealGradient>("darcy_mass_flux_steam_pressure")),
    _darcy_mass_flux_steam_elevation(declareProperty<RealGradient>("darcy_mass_flux_steam_elevation")),
    _Dtau_steamDP(declareProperty<Real>("Dtau_steamDP")),
    _Dtau_steamDH(declareProperty<Real>("Dtau_steamDH")),

    //Equation_of_State_Properties - Non-Derivative Outputs (variables added by Kat)
    _temp_out(declareProperty<Real>("temperature")),
    _sat_fraction_out(declareProperty<Real>("saturation_water")),
    _dens_out(declareProperty<Real>("density")),
    _dens_water_out(declareProperty<Real>("density_water")),
    _dens_steam_out(declareProperty<Real>("density_steam")),
    _enth_water_out(declareProperty<Real>("enthalpy_water")),
    _enth_steam_out(declareProperty<Real>("enthalpy_steam")),
    _visc_water_out(declareProperty<Real>("viscosity_water")),
    _visc_steam_out(declareProperty<Real>("viscosity_steam")),

    //Equation_of_State_Properites - Derivative Outputs (variables added by Kat)
    _d_dens_d_enth(declareProperty<Real>("ddensitydH_P")),
    _d_dens_d_press(declareProperty<Real>("ddensitydp_H")),
    _d_enth_water_d_enth(declareProperty<Real>("denthalpy_waterdH_P")),
    _d_enth_steam_d_enth(declareProperty<Real>("denthalpy_steamdH_P")),
    _d_temp_d_enth(declareProperty<Real>("dTdH_P")),
    _d_sat_fraction_d_enth(declareProperty<Real>("dswdH")),
    _d_enth_water_d_press(declareProperty<Real>("denthalpy_waterdP_H")),
    _d_enth_steam_d_press(declareProperty<Real>("denthalpy_steamdP_H")),
    _d_temp_d_press(declareProperty<Real>("dTdP_H"))
{ }

FluidFlow::~FluidFlow()
{ }

void FluidFlow::computeEOSProperties()
{
  Real _del_press;
  Real _del_enth;
  Real temp_out;
  Real sat_fraction_out;
  Real dens_out, dens_water_out, dens_steam_out;
  Real enth_water_out, enth_steam_out;
  Real visc_water_out, visc_steam_out;
  Real del_press, del_enth;
  Real d_enth_water_d_press, d_enth_steam_d_press;
  Real d_dens_d_press, d_temp_d_press;
  Real d_enth_water_d_enth, d_enth_steam_d_enth;
  Real d_dens_d_enth, d_temp_d_enth, d_sat_fraction_d_enth;

  for(int qp=0; qp<_qrule->n_points(); qp++)
  {
    _water_steam_properties.Equations_of_State_Derivative_Properties(_enthalpy[qp], _pressure[qp], temp_out, sat_fraction_out, dens_out, dens_water_out, dens_steam_out, enth_water_out, enth_steam_out, visc_water_out, visc_steam_out, del_press, del_enth, d_enth_water_d_press, d_enth_steam_d_press, d_dens_d_press, d_temp_d_press, d_enth_water_d_enth, d_enth_steam_d_enth, d_dens_d_enth, d_temp_d_enth, d_sat_fraction_d_enth);

    _temp_out[qp] = temp_out;
    _sat_fraction_out[qp] = sat_fraction_out;
    _dens_out[qp] = dens_out;
    _dens_water_out[qp] = dens_water_out;
    _dens_steam_out[qp] = dens_steam_out;
    _enth_water_out[qp] = enth_water_out;
    _enth_steam_out[qp] = enth_steam_out;
    _visc_water_out[qp] = visc_water_out;
    _visc_steam_out[qp] = visc_steam_out;
    _del_press = del_press;
    _del_enth = del_enth;
    _d_enth_water_d_press[qp] = d_enth_water_d_press;
    _d_enth_steam_d_press[qp] = d_enth_steam_d_press;
    _d_dens_d_press[qp] = d_dens_d_press;
    _d_temp_d_press[qp] = d_temp_d_press;
    _d_enth_water_d_enth[qp] = d_enth_water_d_enth;
    _d_enth_steam_d_enth[qp] = d_enth_steam_d_enth;
    _d_dens_d_enth[qp] = d_dens_d_enth;
    _d_temp_d_enth[qp] = d_temp_d_enth;
    _d_sat_fraction_d_enth[qp] = d_sat_fraction_d_enth;
  }
  
}

void FluidFlow::computeProperties()
{
        for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
        {  
          Real _dens_water0 = 1E3;
            Real _visc_water0 = 5E-4;
            
            // ideal water mode: constant rho, vis and single phase mode   
            if (_has_temp && !_has_enthalpy) 
            {
                _dens_water0 =  _dens_water_out[qp];
                _visc_water0 =  _visc_water_out[qp];
            }
            
            if (!_has_enthalpy) 
            {
                _tau_water[qp] = _permeability[qp] * _dens_water0 / _visc_water0;
                _darcy_mass_flux_water[qp] = -_tau_water[qp] * (_grad_p[qp] + _dens_water0*_gravity[qp]*_gravity_vector[qp]);
                _darcy_mass_flux_water_pressure[qp] =  (-_tau_water[qp] * _grad_p[qp]);
                _darcy_mass_flux_water_elevation[qp] = (-_tau_water[qp] * _gravity[qp] *_gravity_vector[qp]*_dens_water0);
                _darcy_flux_water[qp] = _darcy_mass_flux_water[qp] / _dens_water0;    
            }
            
            // 2 phase:     
            if (_has_enthalpy)
            {
                Real  _tau_water0,  _tau_water1,  _tau_water2;  
                Real  _tau_steam0,  _tau_steam1,  _tau_steam2;
                Real _phase, _temp, _temp_sat, _sat_fraction;
                Real _dens, _dens_water, _dens_steam;
                Real _enth_water, _enth_steam;
                Real _visc_water, _visc_steam;
                Real _del_press, _del_enth;
                Real del_p=1.0, del_h=1e-7;
                Real dpressure=_pressure[qp]+del_p;
                Real denthalpy=_enthalpy[qp]+del_h;
                
                _water_steam_properties.Equations_of_State_Properties(_enthalpy[qp], dpressure, _phase, _temp, _temp_sat, _sat_fraction, _dens, _dens_water, _dens_steam, _enth_water, _enth_steam, _visc_water, _visc_steam, _del_press, _del_enth);
                
                FluidFlow::compute2PhProperties0( _permeability[qp], _sat_fraction, _dens_water, _dens_steam, _visc_water, _visc_steam, _tau_water[qp],_tau_steam[qp]);
                
                _tau_water1 = _tau_water[qp];
                _tau_steam1 = _tau_steam[qp];
                
                
                
                _water_steam_properties.Equations_of_State_Properties(denthalpy, _pressure[qp], _phase, _temp, _temp_sat, _sat_fraction, _dens, _dens_water, _dens_steam, _enth_water, _enth_steam, _visc_water, _visc_steam, _del_press, _del_enth);
                
                FluidFlow::compute2PhProperties0( _permeability[qp], _sat_fraction, _dens_water, _dens_steam, _visc_water, _visc_steam, _tau_water[qp],_tau_steam[qp]);
                
                _tau_water2 = _tau_water[qp];
                _tau_steam2 =  _tau_steam[qp];
                
                
                
                _water_steam_properties.Equations_of_State_Properties( _pressure[qp], _enthalpy[qp], _phase, _temp, _temp_sat, _sat_fraction, _dens, _dens_water, _dens_steam, _enth_water, _enth_steam, _visc_water, _visc_steam, _del_press, _del_enth);
                
                FluidFlow:: compute2PhProperties0( _permeability[qp],_sat_fraction, _dens_water, _dens_steam, _visc_water, _visc_steam, _tau_water[qp],_tau_steam[qp]);
                
                _tau_water0 = _tau_water[qp];
                _tau_steam0 = _tau_steam[qp];
                
                
                
                _darcy_mass_flux_water[qp] =-_tau_water0*(_grad_p[qp] + _dens_water*_gravity[qp]*_gravity_vector[qp]);
                _darcy_mass_flux_steam[qp] =-_tau_steam0*(_grad_p[qp] + _dens_steam*_gravity[qp]*_gravity_vector[qp]);
                _darcy_mass_flux_water_pressure[qp] =  -_tau_water0 * _grad_p[qp];
                _darcy_mass_flux_water_elevation[qp] = -_tau_water0 * _gravity[qp] *_gravity_vector[qp]*_dens_water;
                _darcy_mass_flux_steam_pressure[qp] =  -_tau_steam0 * _grad_p[qp];
                _darcy_mass_flux_steam_elevation[qp] = -_tau_steam0 * _gravity[qp] *_gravity_vector[qp]*_dens_steam; 
                _darcy_flux_steam[qp] = _darcy_mass_flux_steam[qp] /_dens_steam;
                _darcy_flux_water[qp] = _darcy_mass_flux_water[qp] /_dens_water;

                _Dtau_waterDP[qp]=(_tau_water1-_tau_water0)/del_p;
                _Dtau_steamDP[qp]=(_tau_steam1-_tau_steam0)/del_p;
                _Dtau_waterDH[qp]=(_tau_water2-_tau_water0)/del_h;
                _Dtau_steamDH[qp]=(_tau_steam2-_tau_steam0)/del_h;
            }
        }
    }
    
void FluidFlow::compute2PhProperties0(Real _per, Real  _Sw, Real _Denw, Real _Dens, Real _visw, Real _viss, Real & _watertau, Real  &_steamtau)
{
        Real _krw=1.0, _krs=0.0;
        Real _swe;  
        
        // Brooks and Corey  
        _swe=(_Sw-0.3)/0.65;
        if(_swe <= 0.0)
        { _krw= 0.0; }
        else
        { 
            if(_swe >= 1.0)
            { _krw= 1.0; }
            else
            { _krw= _swe*_swe*_swe*_swe; }
        }
        
        if(_swe <= 0.0)
        { _krs= 1.0; }
        else
        { 
            if(_swe >= 1.0)
            {  _krs= 0.0;}
            else
            { _krs= (1-_swe*_swe)*(1-_swe)*(1-_swe); }
        }
        
        // VanG
        /*    _swe=(_Sw-0.3)/0.7; 
         
         if(_swe <= 0.0)
         {_krw= 0.0;}
         else
         {
            if(_swe >= 1.0)
            {_krw= 1.0;}
            else
            {_krw= std::sqrt(_swe)*(1-std::sqrt((1-_swe*_swe)))*(1-std::sqrt((1-_swe*_swe)));
         }
         
         if(_swe <= 0.0)
         {_krs= 1.0;}
         else
         {
            if(_swe >= 1.0)
            {_krs= 0.0;}
            else
            {_krs= std::sqrt(1-_swe)*(1-_swe*_swe); }
         }
         
         */
        // Sorey and others, 1980
        /*  _swe=(_Sw-0.3)/0.65; 
         
         if(_swe <= 0.0)
         {_krw= 0.0;}
         else
         {
            if(_swe >= 1.0)
            {_krw= 1.0;}
            else
            {_krw= _swe*_swe*_swe*_swe; }
         } 
         
         if(_swe <= 0.0)
         {_krs= 1.0;}
         else
         {
            if(_swe >= 1.0)
            {_krs= 0.0;}
            else
            {_krs= 1-_krw; }
         }
         
         */
        // Verma ,1986
        /*  _swe=(_Sw-0.2)/0.695;
         
         if(_swe <= 0.0)
         {_krw= 0.0;}
         else
         {
            if(_swe >= 1.0)
            {_krw= 1.0;}
            else
            {_krw= _swe*_swe*_swe;}
         }
      
         if(_swe <= 0.0)
         {_krs= 1.0;}
         else
         {
            if(_swe >= 1.0)
            {_krs= 0.0; }
            else
            {_krs=1.2984-1.9832*_swe+0.7432* _swe*_swe; }
         }
         */
        
        _watertau = _per* _Denw / _visw * _krw;  
        _steamtau = _per* _Dens / _viss * _krs;
    }



