#ifndef PRESSUREFUNCTION_H
#define PRESSUREFUNCTION_H

#include "Function.h"

class PressureFunction;

template<>
InputParameters validParams<PressureFunction>();

class PressureFunction : public Function
{
 public:
  PressureFunction(const std::string & name, InputParameters parameters);

  virtual Real value(Real t, const Point & p);

 protected:
  Real _well_radius;
  std::vector<Real> _schedule;
  std::vector<Real> mass_flow_rate;
  Real _surface_fluid_density;
  Real _fluid_viscosity;
  Real _depth_return_pressure;
  std::vector<Real> well_head_pressure;
  Real _gravity;
  Real _surface_roughness;
};

#endif //PRESSUREFUNCTION_H
