#include "ThermalGradientEarthFunction.h"

template<>
InputParameters validParams<ThermalGradientEarthFunction>()
{
  InputParameters params = validParams<Function>();
  params.addParam<Real>("surface_earth_temperature", 281.15, "The surface of the earth temp [m] ");
  params.addParam<Real>("reservoir_thermal_gradient", 0.2, "thermal gradient of the reservoir [K/m]");
  return params;
}

ThermalGradientEarthFunction::ThermalGradientEarthFunction(const std::string & name, InputParameters parameters):
  Function(name, parameters),
  _surface_earth_temperature(getParam<Real>("surface_earth_temperature")),
  _reservoir_thermal_gradient(getParam<Real>("reservoir_thermal_gradient"))
{}

Real
ThermalGradientEarthFunction::value(Real /*t*/, const Point & p)
{
  std::cout << "returned earth temp: " << _surface_earth_temperature + _reservoir_thermal_gradient * p(0) << std::endl;
  return _surface_earth_temperature + _reservoir_thermal_gradient * p(0);
}
