#ifndef THERMALGRADIENTEARTHFUNCTION_H
#define THERMALGRADIENTEARTHFUNCTION_H

#include "Function.h"

class ThermalGradientEarthFunction;

template<>
InputParameters validParams<ThermalGradientEarthFunction>();

class ThermalGradientEarthFunction:public Function
{
 public:
  ThermalGradientEarthFunction(const std::string & name, InputParameters parameters);

  virtual Real value(Real t, const Point & p);

 protected:
  Real _surface_earth_temperature;
  Real _reservoir_thermal_gradient;

};

#endif //THERMALGRADIENTEARTHFUNCTION_H
