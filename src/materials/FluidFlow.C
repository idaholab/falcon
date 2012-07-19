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
#include "Water_Steam_EOS.h"   
//#include "RelativePermeability.h"

template<>
InputParameters validParams<FluidFlow>()
{
  InputParameters params = validParams<PorousMedia>(); 
  params.addCoupledVar("pressure", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("enthalpy", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
  params.addCoupledVar("density_water", "Coupled NodalAux used to calculate density");
  params.addCoupledVar("viscosity_water", "Coupled NodalAux used to calculate viscosity");
  return params;
}

FluidFlow::FluidFlow(const std::string & name,
                     InputParameters parameters)
  :PorousMedia(name, parameters),
   _has_pressure(isCoupled("pressure")),
   _grad_p(_has_pressure ? coupledGradient("pressure") : _grad_zero),
   _pressure(_has_pressure ? coupledValue("pressure")  : _zero),

   _has_temp(isCoupled("temperature")),
   _temperature(_has_temp ? coupledValue("temperature")  : _zero),
   
   _density_water(_has_temp ? coupledValue("density_water") : _zero),   //nodal Aux
   _viscosity_water(_has_temp ? coupledValue("viscosity_water") : _zero), //nodal Aux

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
   _Dtau_steamDH(declareProperty<Real>("Dtau_steamDH"))

{ }


void FluidFlow::computeProperties()
{
for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
 {  
   Real _dens_water = 1E3;
   Real _visc_water = 5E-4;
// ideal water mode: constant rho, vis and single phase mode   
    if (_has_temp && !_has_enthalpy) 
    {
        _dens_water =  _density_water[qp];
        _visc_water =  _viscosity_water[qp];
    }
 
    if (!_has_enthalpy) 
    {_tau_water[qp] = _permeability[qp] * _dens_water / _visc_water;
    _darcy_mass_flux_water[qp] = -_tau_water[qp] * (_grad_p[qp] + _dens_water*_gravity[qp]*_gravity_vector[qp]);
    _darcy_mass_flux_water_pressure[qp] =  (-_tau_water[qp] * _grad_p[qp]);
    _darcy_mass_flux_water_elevation[qp] = (-_tau_water[qp] * _gravity[qp] *_gravity_vector[qp]*_dens_water);
    _darcy_flux_water[qp] = _darcy_mass_flux_water[qp] / _dens_water;    
    }
     
// 2 phase:     
    if (_has_enthalpy)
  {
    Real  _tau_water0,  _tau_water1,  _tau_water2;  
    Real  _tau_steam0,  _tau_steam1,  _tau_steam2;
    Real _T, _Sw, _Den, _Denw, _Dens, _hw, _hs, _visw, _viss;
    int  _ierror;
    Real del_p=1.0, del_h=1e-7;
    Real dpressure=_pressure[qp]+del_p;
    Real denthalpy=_enthalpy[qp]+del_h;
    
    Water_Steam_EOS::water_steam_prop_ph_noderiv_(dpressure, _enthalpy[qp],  
                                                 _T, _Sw, _Den, _Denw, _Dens, _hw, _hs, _visw, _viss, _ierror);
    FluidFlow:: compute2PhProperties0( _permeability[qp], _Sw,_Denw, _Dens, _visw, _viss, _tau_water[qp],_tau_steam[qp]);
    _tau_water1 = _tau_water[qp];
    _tau_steam1 = _tau_steam[qp];
    Water_Steam_EOS::water_steam_prop_ph_noderiv_(_pressure[qp], denthalpy,  
                                                 _T, _Sw, _Den, _Denw, _Dens, _hw, _hs, _visw, _viss, _ierror);
    FluidFlow:: compute2PhProperties0( _permeability[qp], _Sw,_Denw, _Dens, _visw, _viss,  _tau_water[qp], _tau_steam[qp]);
    _tau_water2 = _tau_water[qp];
    _tau_steam2 =  _tau_steam[qp];
    Water_Steam_EOS::water_steam_prop_ph_noderiv_( _pressure[qp], _enthalpy[qp],
                                                 _T, _Sw, _Den, _Denw, _Dens, _hw, _hs, _visw, _viss, _ierror);
    FluidFlow:: compute2PhProperties0( _permeability[qp],_Sw,_Denw, _Dens, _visw, _viss, _tau_water[qp], _tau_steam[qp]);
    _tau_water0 = _tau_water[qp];
    _tau_steam0 = _tau_steam[qp];
    _darcy_mass_flux_water[qp] =-_tau_water0*(_grad_p[qp] + _Denw*_gravity[qp]*_gravity_vector[qp]);
    _darcy_mass_flux_steam[qp] =-_tau_steam0*(_grad_p[qp] + _Dens*_gravity[qp]*_gravity_vector[qp]);
    _darcy_mass_flux_water_pressure[qp] =  -_tau_water0 * _grad_p[qp];
    _darcy_mass_flux_water_elevation[qp] = -_tau_water0 * _gravity[qp] *_gravity_vector[qp]*_Denw;
    _darcy_mass_flux_steam_pressure[qp] =  -_tau_steam0 * _grad_p[qp];
    _darcy_mass_flux_steam_elevation[qp] = -_tau_steam0 * _gravity[qp] *_gravity_vector[qp]*_Dens; 
    _darcy_flux_steam[qp] = _darcy_mass_flux_steam[qp] /_Dens;
    _darcy_flux_water[qp] = _darcy_mass_flux_water[qp] /_Denw;

    _Dtau_waterDP[qp]=(_tau_water1-_tau_water0)/del_p;
    _Dtau_steamDP[qp]=(_tau_steam1-_tau_steam0)/del_p;
    _Dtau_waterDH[qp]=(_tau_water2-_tau_water0)/del_h;
    _Dtau_steamDH[qp]=(_tau_steam2-_tau_steam0)/del_h;
    
  }    
 }
}



    
void FluidFlow::compute2PhProperties0(Real _per, Real  _Sw,Real _Denw, Real _Dens, Real _visw, Real _viss, Real
  & _watertau, Real  &_steamtau)
{
    Real _krw=1.0, _krs=0.0;
    Real _swe;  
// Brooks and Corey  
    _swe=(_Sw-0.3)/0.65;
    if(_swe <= 0.0)
    { _krw= 0.0; }
    else
    { if(_swe >= 1.0)
      { _krw= 1.0; }
      else
      { _krw= _swe*_swe*_swe*_swe; }
    }
    
    if(_swe <= 0.0)
    { _krs= 1.0; }
    else
    { if(_swe >= 1.0)
      {  _krs= 0.0;}
      else
      { _krs= (1-_swe*_swe)*(1-_swe)*(1-_swe); }
    }

// VanG
    /*    _swe=(_Sw-0.3)/0.7; 
      if(_swe <= 0.0)
      {_krw= 0.0;}
      else
       {if(_swe >= 1.0)
         {_krw= 1.0;}
         else
         {_krw= std::sqrt(_swe)*(1-std::sqrt((1-_swe*_swe)))*(1-std::sqrt((1-_swe*_swe))); }
       }
      
      if(_swe <= 0.0)
      {_krs= 1.0;}
      else
      {if(_swe >= 1.0)
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
       {if(_swe >= 1.0)
       {_krw= 1.0;}
         else
         {_krw= _swe*_swe*_swe*_swe; }
       } 
      if(_swe <= 0.0)
      {_krs= 1.0;}
      else
      {if(_swe >= 1.0)
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
       {if(_swe >= 1.0)
       {_krw= 1.0;}
         else
         {_krw= _swe*_swe*_swe;}
       }
      
      if(_swe <= 0.0)
      {_krs= 1.0;}
      else
      {if(_swe >= 1.0)
      {_krs= 0.0; }
      else
      {_krs=1.2984-1.9832*_swe+0.7432* _swe*_swe; }
      }

    */
      
    
    _watertau = _per* _Denw / _visw * _krw;  
    _steamtau = _per* _Dens / _viss * _krs;
     
}



