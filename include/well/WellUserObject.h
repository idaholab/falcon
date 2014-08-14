#ifndef WELLUSEROBJECT_H
#define WELLUSEROBJECT_H

#include "GeneralUserObject.h"

class WellUserObject;

template<>
InputParameters validParams<WellUserObject>();

class WellUserObject : public GeneralUserObject
{
 public:
  WellUserObject(const std::string & name, InputParameters params);

  virtual ~WellUserObject();

  virtual void initialize(){}

  virtual void execute(){}
  virtual void finalize(){}

  Real WellBottomTemperature ( Real _well_depth, Real _surface_earth_temperature, Real _surface_fluid_density, Real _fluid_heat_capacity, Real _thermal_conductivity_earth, Real _thermal_conductivity_casing, Real _thermal_conductivity_cement, Real _thermal_conductivity_water, Real _well_radius, Real _casing_thickness, Real _cement_thickness, Real _reservoir_thickness, Real _reservoir_thermal_gradient, Real _return_depth_T_P, Real _mass_flow_rate, Real _surface_fluid_temperature, Real& T ) const;

  Real WellBottomPressure ( Real _well_depth, Real _surface_fluid_density, Real _well_radius, Real _return_depth_T_P, Real _mass_flow_rate, Real _surface_fluid_viscosity, Real _well_head_pressure, Real _gravity, Real _surface_roughness,  Real& P) const;

};
#endif /* WELLUSEROBJECT_H */
