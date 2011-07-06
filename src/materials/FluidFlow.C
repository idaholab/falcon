#include "FluidFlow.h"
//#include "RelativePermeability.h"

template<>
InputParameters validParams<FluidFlow>()
{
  InputParameters params = validParams<PorousMedia>();
//these coupled variables are used to calculate some fluid mass flux related quantities only  
  params.addCoupledVar("pressure", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("enthalpy", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
  params.addCoupledVar("density_water", "Coupled NodalAux used to calculate density");
  params.addCoupledVar("viscosity_water", "Coupled NodalAux used to calculate viscosity");
  params.addCoupledVar("density_steam", "Coupled NodalAux used to calculate density");
  params.addCoupledVar("viscosity_steam", "Coupled NodalAux used to calculate viscosity");
  params.addCoupledVar("saturation_water", "Coupled NodalAux used to calculate relative permeability");
  return params;
}

FluidFlow::FluidFlow(const std::string & name,
                     InputParameters parameters)
  :PorousMedia(name, parameters),
   _has_pressure(isCoupled("pressure")),
   _grad_p  (_has_pressure ? coupledGradient("pressure") : _grad_zero),
   _pressure(_has_pressure ? coupledValue("pressure")  : _zero),

   _has_temp(isCoupled("temperature")),
   _temperature(_has_temp ? coupledValue("temperature")  : _zero),
   
   _density_water(_has_temp ? coupledValue("density_water") : _zero),   //nodal Aux
   _viscosity_water(_has_temp ? coupledValue("viscosity_water") : _zero), //nodal Aux

   _has_enthalpy(isCoupled("enthalpy")), 
   _density_steam(_has_enthalpy? coupledValue("density_steam"): _zero),   //nodal Aux
   _viscosity_steam(_has_enthalpy? coupledValue("viscosity_steam"): _zero), //nodal Aux

   _saturation_water(_has_enthalpy? coupledValue("saturation_water"): _zero), //nodal Aux

   _tau_water(declareProperty<Real>("tau_water")),
   _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
   _darcy_mass_flux_water(declareProperty<RealGradient>("darcy_mass_flux_water")),
   _darcy_mass_flux_water_pressure(declareProperty<RealGradient>("darcy_mass_flux_water_pressure")),
   _darcy_mass_flux_water_elevation(declareProperty<RealGradient>("darcy_mass_flux_water_elevation")),

   _tau_steam(declareProperty<Real>("tau_steam")),
   _darcy_flux_steam(declareProperty<RealGradient>("darcy_flux_steam")),
   _darcy_mass_flux_steam(declareProperty<RealGradient>("darcy_mass_flux_steam")),
   _darcy_mass_flux_steam_pressure(declareProperty<RealGradient>("darcy_mass_flux_steam_pressure")),
   _darcy_mass_flux_steam_elevation(declareProperty<RealGradient>("darcy_mass_flux_steam_elevation"))

{ }

void
FluidFlow::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {     
//Calculate flow related quantities
    Real _krw=1.0, _krs=0.0;
    Real _swe;  // effective water saturation 
    Real _dens_water = 1E3;
    Real _visc_water = 5E-4;  
// relative_permeability :: relative_permeability_noderiv1_(_saturation_water[qp],_krw,_krs);   
 /*
      _swe=(_saturation_water[qp]-0.3)/0.7;
      
      if(_swe <= 0.0)
        {_krw= 0.0;}
      else
       {if(_swe >= 1.0)
          {_krw= 1.0; }
         else
         {_krw= _swe*_swe; _krw= _krw*_krw;}
       }

      _swe=(1.0-_saturation_water[qp]-0.2)/0.8;
      
      if(_swe <= 0.0)
      {_krs= 0.0;}
      else
      {if(_swe >= 1.0)
      {_krs= 1.0; }
      else
      {_krs= _swe*_swe;_krs=_krs*_krs; }
      }
 */
    if (_has_enthalpy)
    {
      _krw=_saturation_water[qp];
      _krs=(1.-_krw);
    }
    else
    {
      _krw =1.0;
      _krs=0.0;
    }

  // simplified version for now
    
    if (_has_temp)
    {
        _dens_water =  _density_water[qp];
        _visc_water =  _viscosity_water[qp];
    }
      
    _tau_water[qp] = _permeability[qp] * _dens_water / _visc_water * _krw;
    _darcy_mass_flux_water[qp] =  -_tau_water[qp] * (_grad_p[qp]+_dens_water*_gravity[qp]*_gravity_vector[qp]);
    _darcy_mass_flux_water_pressure[qp] =  (-_tau_water[qp] * _grad_p[qp]);
    _darcy_mass_flux_water_elevation[qp] = (-_tau_water[qp] * _gravity[qp] *_gravity_vector[qp]*_dens_water);
    _darcy_flux_water[qp] = _darcy_mass_flux_water[qp] /   _dens_water;
 
          
    if (_has_enthalpy)
    {_tau_steam[qp] = _permeability[qp] * _density_steam[qp] / _viscosity_steam[qp] * _krs;
     _darcy_mass_flux_steam[qp] =  -_tau_steam[qp] * (_grad_p[qp]+_density_steam[qp]*_gravity[qp]*_gravity_vector[qp]);
     _darcy_mass_flux_steam_pressure[qp] =  (-_tau_steam[qp] * _grad_p[qp]);
     _darcy_mass_flux_steam_elevation[qp] = (-_tau_steam[qp] * _gravity[qp] *_gravity_vector[qp]*_density_steam[qp]);
     _darcy_flux_steam[qp] = _darcy_mass_flux_steam[qp] /   _density_steam[qp];
    } 
    
   //  if( _q_point[qp](0) <= 1)// || _q_point[qp](0) >= 99)
   //  { std::cout << "kr: "<<_q_point[qp](0) <<" "<< _darcy_flux_water[qp]<<" "  <<_darcy_flux_steam[qp]<<"\n";}
     
    // std::cout << "kr: "<< _darcy_flux_steam[qp]<<  "\n";
    
  }
}



