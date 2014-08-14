#ifndef WELLDIRAC_H
#define WELLDIRAC_H

#include "DiracKernel.h"
#include "Function.h"
#include "WaterSteamEOS.h"
#include "WellUserObject.h"

class WellDirac;

template<>
InputParameters validParams<WellDirac>();

class WellDirac : public DiracKernel
{
 public:
  WellDirac(const std::string & name, InputParameters parameters);

  virtual void addPoints();
  virtual Real computeQpResidual();

 protected:
  const WaterSteamEOS & _water_steam_properties;
  const WellUserObject & _temperature_pressure;
  Point _p;
  std::vector<Real> _point_param;
  std::vector<Real> _schedule;
  std::vector<Real> mass_flow_rate;
  Real _well_radius;
  Real _surface_earth_temperature;
  Real _fluid_heat_capacity;
  Real _thermal_conductivity_earth;
  Real _thermal_conductivity_casing;
  Real _thermal_conductivity_cement;
  Real _thermal_conductivity_water;
  Real _well_depth;
  Real _casing_thickness;
  Real _cement_thickness;
  Real _reservoir_thickness;
  Real _reservoir_thermal_gradient;
  Real _return_depth_T_P;
  std::vector<Real> surface_fluid_temperature;
  std::vector<Real> well_head_pressure;
  Real _gravity;
  int _return_parameter;    
  Real _surface_roughness;
};

#endif //WELLDIRAC_H                                                                                                                                                



