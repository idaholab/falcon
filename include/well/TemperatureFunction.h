#ifndef TEMPERATUREFUNCTION_H
#define TEMPERATUREFUNCTION_H

#include "Function.h"

class TemperatureFunction;

template<>
InputParameters validParams<TemperatureFunction>();

class TemperatureFunction:public Function
{
 public:
    TemperatureFunction(const std::string & name, InputParameters parameters);

    virtual Real value(Real t, const Point & p);

 protected:
    Real _surface_earth_temperature;
    Real _surface_fluid_density;
    Real _fluid_heat_capacity;
    Real _thermal_conductivity_earth;
    Real _thermal_conductivity_casing;
    Real _thermal_conductivity_cement;
    Real _thermal_conductivity_water;
    Real _well_radius;
    Real _well_depth;
    Real _casing_thickness;
    Real _cement_thickness;
    Real _reservoir_thickness;
    Real _reservoir_thermal_gradient;
    Real _temp_return_depth;
    std::vector<Real> _schedule;
    std::vector<Real> mass_flow_rate;
    std::vector<Real> surface_fluid_temperature;
};

#endif //TEMPERATUREFUNCTION_H
