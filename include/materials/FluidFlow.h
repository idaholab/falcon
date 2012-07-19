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

#ifndef FLUIDFLOW_H
#define FLUIDFLOW_H

#include "PorousMedia.h"

//Forward Declarations
class FluidFlow;
class Banana_Slug;

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
  virtual ~FluidFlow();
  
protected:
    virtual void computeEOSProperties();
    virtual void computeProperties();
    virtual void compute2PhProperties0(Real _per, Real  _Sw,Real _Denw, Real _Dens, Real _visw, Real _viss, Real &_watertau, Real  &_steamtau);

  const Banana_Slug & _water_steam_properties;
  bool _has_pressure;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;

  bool _has_temp;
  //VariableValue & temp_out;
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
    
  //Equation_of_State_Properties - Non-Derivative Material Outputs (added by Kat)
  //MaterialProperty<Real> & _enth_in;
  //MaterialProperty<Real> & _press_in;
  MaterialProperty<Real> & _temp_out;
  MaterialProperty<Real> & _sat_fraction_out;
  MaterialProperty<Real> & _dens_out;
  MaterialProperty<Real> & _dens_water_out;
  MaterialProperty<Real> & _dens_steam_out;
  MaterialProperty<Real> & _enth_water_out;
  MaterialProperty<Real> & _enth_steam_out;
  MaterialProperty<Real> & _visc_water_out;
  MaterialProperty<Real> & _visc_steam_out;
    
  //Equations_of_State_Properties - Derivative Material Ouptuts (added by Kat)
  MaterialProperty<Real> & _d_dens_d_enth;
  MaterialProperty<Real> & _d_dens_d_press;
  MaterialProperty<Real> & _d_enth_water_d_enth;
  MaterialProperty<Real> & _d_enth_steam_d_enth;
  MaterialProperty<Real> & _d_temp_d_enth;
  MaterialProperty<Real> & _d_sat_fraction_d_enth;
  MaterialProperty<Real> & _d_enth_water_d_press;
  MaterialProperty<Real> & _d_enth_steam_d_press;
  MaterialProperty<Real> & _d_temp_d_press;

    
};

#endif //FLUIDFLOW_H
