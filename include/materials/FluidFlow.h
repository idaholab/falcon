#ifndef FLUIDFLOW_H
#define FLUIDFLOW_H

#include "PorousMedia.h"


//Forward Declarations
class FluidFlow;

template<>
InputParameters validParams<FluidFlow>();

/**
 * Simple material with FluidFlow properties.
 */
class FluidFlow : virtual public PorousMedia
{
public:
  FluidFlow(const std::string & name,
            InputParameters parameters);
  
protected:
  virtual void computeProperties();
  virtual void compute2PhProperties0(Real _per, Real  _Sw,Real _Denw, Real _Dens, Real _visw, Real _viss, Real &_watertau, Real  &_steamtau);
  
  
  
  bool _has_pressure;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;

  bool _has_temp;
  VariableValue  & _temperature;
 
  VariableValue & _density_water;
  VariableValue & _viscosity_water;
  
  bool _has_enthalpy;  
  VariableValue & _enthalpy;

  MaterialProperty<Real> & _tau_water;
  MaterialProperty<RealGradient> & _darcy_flux_water;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water_pressure;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water_elevation;
  MaterialProperty<Real> & _Dtau_waterDP;
  MaterialProperty<Real> & _Dtau_waterDH;
  
  MaterialProperty<Real> & _tau_steam;
  MaterialProperty<RealGradient> & _darcy_flux_steam;
  MaterialProperty<RealGradient> & _darcy_mass_flux_steam;
  MaterialProperty<RealGradient> & _darcy_mass_flux_steam_pressure;
  MaterialProperty<RealGradient> & _darcy_mass_flux_steam_elevation;
  MaterialProperty<Real> & _Dtau_steamDP;
  MaterialProperty<Real> & _Dtau_steamDH;
    
};

#endif //FLUIDFLOW_H
