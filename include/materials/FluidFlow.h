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

  bool _has_pressure;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;

// VariableValue  & _density_water;
  bool _has_temp;
  VariableValue  & _temperature;
 
  VariableValue & _density_water;
  VariableValue & _viscosity_water;
  
  bool _has_enthalpy;  
  VariableValue & _density_steam;
  VariableValue & _viscosity_steam;
    
  VariableValue & _saturation_water;
  

  MaterialProperty<Real> & _tau_water;
  MaterialProperty<RealGradient> & _darcy_flux_water;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water_pressure;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water_elevation;

  MaterialProperty<Real> & _tau_steam;
  MaterialProperty<RealGradient> & _darcy_flux_steam;
  MaterialProperty<RealGradient> & _darcy_mass_flux_steam;
  MaterialProperty<RealGradient> & _darcy_mass_flux_steam_pressure;
  MaterialProperty<RealGradient> & _darcy_mass_flux_steam_elevation;
    
};

#endif //FLUIDFLOW_H
