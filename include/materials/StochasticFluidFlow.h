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

#ifndef STOCHASTICFLUIDFLOW_H
#define STOCHASTICFLUIDFLOW_H

#include "StochasticPorousMedia.h"


//Forward Declarations
class StochasticFluidFlow;

template<>
InputParameters validParams<StochasticFluidFlow>();

/**
 * Simple material with StochasticFluidFlow properties.
 */
class StochasticFluidFlow : virtual public StochasticPorousMedia
{
public:
  StochasticFluidFlow(const std::string & name,
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

#endif //STOCHASTICFLUIDFLOW_H
