#include "PressureFunction.h"

template<>
InputParameters validParams<PressureFunction>()
{
  InputParameters params = validParams<Function>();
  params.addParam<Real>("well_radius", 0.127, "radius of the well [m]");
  params.addParam<std::vector<Real> >("schedule", std::vector<Real>(0.0), "The times each injection stage starts");
  params.addParam<std::vector<Real> >("mass_flow_rate", std::vector<Real>(0.0), "The injection range at each stage");
  params.addParam<Real>("_surface_fluid_density", 983.2, "density of fluid, [kg/m^3]");
  params.addParam<Real>("fluid_viscosity", 0.65e-3, "viscosity of fluid [Pa.s]");
  params.addParam<Real>("depth_return_pressure", 1800, "depth to return pressure [m]");
  params.addParam<std::vector<Real> >("well_head_pressure", std::vector<Real>(0.0), "pressure at wellhead [Pa]");
  params.addParam<Real>("gravity", 9.81, "Gravity [m/s^2]");
  params.addParam<Real>("surface_roughness", 0.0000457, "Surface roughness parameter for the well in turbulent regime. Get from table or chart.");
  return params;
}

PressureFunction::PressureFunction(const std::string & name, InputParameters parameters):
    Function(name, parameters),
    _well_radius(getParam<Real>("well_radius")),
    _schedule(getParam<std::vector<Real> >("schedule")),
    mass_flow_rate(getParam<std::vector<Real> >("mass_flow_rate")),
    _surface_fluid_density(getParam<Real>("_surface_fluid_density")),
    _fluid_viscosity(getParam<Real>("fluid_viscosity")),
    _depth_return_pressure(getParam<Real>("depth_return_pressure")),
    well_head_pressure(getParam<std::vector<Real> >("well_head_pressure")),
    _gravity(getParam<Real>("gravity")),
    _surface_roughness(getParam<Real>("surface_roughness"))
{}

Real
PressureFunction::value(Real /*t*/, const Point & p)
{
  int _nstages = _schedule.size();
  int _size_mass_flow_array = mass_flow_rate.size();
  Real _mass_flow_rate;
  Real _well_head_pressure;

  if (_nstages > 1)
  {
    for (unsigned int i=0; i < _nstages; i++)
    {
      if(_t >= _schedule[i] and _t < _schedule[i+1])
      {
        _mass_flow_rate = mass_flow_rate[i];
        _well_head_pressure = well_head_pressure[i];

        break;

      }
    }

    if (_t >= _schedule[_nstages - 1])
    {
      _mass_flow_rate = mass_flow_rate[_nstages -1];
      _well_head_pressure = well_head_pressure[_nstages -1];
    }


    else if ( _t <= _schedule[0] )
    {
      _mass_flow_rate = mass_flow_rate[0];
      _well_head_pressure = well_head_pressure[0];
    }
  }

  //calculate diameter of wellbore
  double diameter = 2 * _well_radius;

  //calculate fluid velocity from mass flow rate
  double velocity = ( _mass_flow_rate / _surface_fluid_density ) / ( libMesh::pi*_well_radius*_well_radius );

  //calculate the reynold's number
  double Re = ( diameter*velocity*_surface_fluid_density) / _fluid_viscosity;
  double f;

  //calculate the friction factor
  if (Re == 0)
  {
    f = 0;
  }
  else if (Re >= 4000)
  {
    f = 0.001375*(1+std::pow((20000*(_surface_roughness/diameter)+1000000/Re),1/3.));
  }
  else
  {
    f = 16 / Re;
  }

  //calculate friction
  double F = 4*f* (( _depth_return_pressure ) / diameter) * ((velocity*velocity) / 2);

  //calculate P
  double P = _well_head_pressure + _surface_fluid_density*_gravity*_depth_return_pressure - _surface_fluid_density*F;
  return P;
}
