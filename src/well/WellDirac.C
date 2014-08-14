//This dirac kernel takes all common well parameters as input and calculates the mass flow rate and enthalpy out of the bottom of the cased portion into
//the fractured portion of the well. 
#include "WellDirac.h"

template<>
InputParameters validParams<WellDirac>()
{
  InputParameters params = validParams<DiracKernel>();
  params.addParam<UserObjectName>("water_steam_properties", "EOS functions, calculates water steam properties");
  params.addParam<UserObjectName>("temperature_pressure", "Temperature and Pressure functions, calculates temperature and pressure downhole");
  params.addRequiredParam<std::vector<Real> >("point", "The x,y,z coordinates of the point");
  params.addParam<std::vector<Real> >("schedule", std::vector<Real>(0.0), "The times each injection stage starts");
  params.addParam<std::vector<Real> >("mass_flow_rate", std::vector<Real>(0.0), "The injection range at each stage");
  params.addParam<Real>("well_radius", 0.127, "radius of the well [m]");
  params.addParam<Real>("surface_earth_temperature", 315, "surface earth temperature, [K]");
  params.addParam<Real>("fluid_heat_capacity", 4185, "heat capacity of fluid, [J/(kg.K)]");
  params.addParam<Real>("thermal_conductivity_earth", 1.5, "thermal conductivity of earth, [W/(m.K)]");
  params.addParam<Real>("thermal_conductivity_casing", 43, "thermal conductivity of casing [W/(m.K)]");
  params.addParam<Real>("thermal_conductivity_cement", 1.4, "thermal conductivity of well cement [W/(m.K)]");
  params.addParam<Real>("thermal_conductivity_water", 0.58, "thermal conductivity of water [W/(m.K)]");
  params.addParam<Real>("well_depth", 2000, "depth of the well [m]");
  params.addParam<Real>("casing_thickness", 0.038, "thickness of the casing [m] (enter zero if no casing)");
  params.addParam<Real>("cement_thickness", 0.051, "thickness of the cement [m] (enter zero if no cement)");
  params.addParam<Real>("reservoir_thickness", 100, "thickness of the reservoir [m]");
  params.addParam<Real>("reservoir_thermal_gradient", 0.02, "thermal gradient of the reservoir [K/m]");
  params.addParam<Real>("return_depth_T_P", 1800, "depth to calculate temperature and pressure [m]");
  params.addParam<std::vector<Real> >("surface_fluid_temperature", std::vector<Real>(0.0), "The temperature of the injection at each stage");
  params.addParam<std::vector<Real> >("well_head_pressure", std::vector<Real>(0.0), "pressure at wellhead [Pa]");
  params.addParam<Real>("gravity", 9.81, "Gravity [m/s^2]");
  params.addParam<int>("return_parameter", 1, "1 returns mass flow rate [kg/s], 2 returns enthalpy, 3 returns temperature [K], and 4 returns pressure [Pa]");
  params.addParam<Real>("surface_roughness", 0.0000457, "Surface roughness parameter for the well in the turbulent regime.  Get from chart or table. ");
  return params;
}

WellDirac::WellDirac(const std::string & name, InputParameters parameters) :
  DiracKernel(name, parameters),
  _water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),
  _temperature_pressure(getUserObject<WellUserObject>("temperature_pressure")),
  _point_param(getParam<std::vector<Real> >("point")),
  _schedule(getParam<std::vector<Real> >("schedule")),
  mass_flow_rate(getParam<std::vector<Real> >("mass_flow_rate")),
  _well_radius(getParam<Real>("well_radius")),
  _surface_earth_temperature(getParam<Real>("surface_earth_temperature")),
  _fluid_heat_capacity(getParam<Real>("fluid_heat_capacity")),
  _thermal_conductivity_earth(getParam<Real>("thermal_conductivity_earth")),
  _thermal_conductivity_casing(getParam<Real>("thermal_conductivity_casing")),
  _thermal_conductivity_cement(getParam<Real>("thermal_conductivity_cement")),
  _thermal_conductivity_water(getParam<Real>("thermal_conductivity_water")),
  _well_depth(getParam<Real>("well_depth")),
  _casing_thickness(getParam<Real>("casing_thickness")),
  _cement_thickness(getParam<Real>("cement_thickness")),
  _reservoir_thickness(getParam<Real>("reservoir_thickness")),
  _reservoir_thermal_gradient(getParam<Real>("reservoir_thermal_gradient")),
  _return_depth_T_P(getParam<Real>("return_depth_T_P")),
  surface_fluid_temperature(getParam<std::vector<Real> >("surface_fluid_temperature")),
  well_head_pressure(getParam<std::vector<Real> >("well_head_pressure")),
  _gravity(getParam<Real>("gravity")),
  _return_parameter(getParam<int>("return_parameter")),
  _surface_roughness(getParam<Real>("surface_roughness"))

{
  _p(0) = _point_param[0];

  if (_point_param.size() > 1)
    {
      _p(1) = _point_param[1];

      if (_point_param.size() > 2)
        {
          _p(2) = _point_param[2];
        }
    }
}

void
WellDirac::addPoints()
{
  addPoint(_p);
}

Real
WellDirac::computeQpResidual()
{
  int _nstages = _schedule.size();
  int _size_temp_array = surface_fluid_temperature.size();
  int _size_mass_flow_array = mass_flow_rate.size();
  int _size_well_head_pressure_array = well_head_pressure.size();
  
  Real _mass_flow_rate;
  Real _surface_fluid_temperature;
  Real _well_head_pressure;

  if (_nstages > 1)
    {
      for (unsigned int i=0; i < _nstages; i++)
	{
	  if(_t >= _schedule[i] and _t < _schedule[i+1])
	    {
	      _mass_flow_rate = mass_flow_rate[i];
	      _surface_fluid_temperature = surface_fluid_temperature[i];
	      _well_head_pressure = well_head_pressure[i];
	      break;
	    }
	}

      if (_t >= _schedule[_nstages - 1])
	{
	  _mass_flow_rate = mass_flow_rate[_nstages -1];
	  _surface_fluid_temperature = surface_fluid_temperature[_nstages -1];
	  _well_head_pressure = well_head_pressure[ _nstages-1];
	}

      else if ( _t <= _schedule[0] )
	{
	  _mass_flow_rate = mass_flow_rate[0];
	  _surface_fluid_temperature = surface_fluid_temperature[0];
	  _well_head_pressure = well_head_pressure[0];
	}
    }

  //calculate area of wellbore                                                                                                                                  
  Real _area_well_bore = libMesh::pi*_well_radius*_well_radius;

  //Calculate surface fluid density
  Real _surface_fluid_density;
  Real _surface_fluid_viscosity;
  Real _dummy_var;
  _water_steam_properties.waterEquationOfStatePT ( _well_head_pressure, _surface_fluid_temperature, _dummy_var, _surface_fluid_density );
  _water_steam_properties.viscosity( _surface_fluid_density, _surface_fluid_temperature, _surface_fluid_viscosity );

  //Get well bottom temperature and pressure from WellUserObject.C
  Real T;
  Real P;

  _temperature_pressure.WellBottomTemperature( _well_depth, _surface_earth_temperature, _surface_fluid_density, _fluid_heat_capacity, _thermal_conductivity_earth, _thermal_conductivity_casing, _thermal_conductivity_cement, _thermal_conductivity_water, _well_radius, _casing_thickness, _cement_thickness, _reservoir_thickness, _reservoir_thermal_gradient, _return_depth_T_P, _mass_flow_rate, _surface_fluid_temperature,  T );

  _temperature_pressure.WellBottomPressure( _well_depth, _surface_fluid_density, _well_radius, _return_depth_T_P, _mass_flow_rate, _surface_fluid_viscosity, _well_head_pressure, _gravity, _surface_roughness, P );

  if (_return_parameter == 1 )
    {
      //Get density                                                                                                                                       
      Real _dummy_var;
      Real _fluid_density;

      _water_steam_properties.waterEquationOfStatePT ( P, T, _dummy_var, _fluid_density);

      //Calculate mass flow rate from new density              
      Real _velocity = ( _mass_flow_rate / _surface_fluid_density ) / ( libMesh::pi*_well_radius*_well_radius );
      Real _new_mass_flow_rate;
      _new_mass_flow_rate = _fluid_density*_area_well_bore*_velocity;

      return -_test[_i][_qp]*_new_mass_flow_rate;
    }
  else if (_return_parameter == 2 )
    {
      //Get enthalpy                                                                                                                                        
      Real _dummy_var;
      Real _fluid_enthalpy;
      _water_steam_properties.waterEquationOfStatePT ( P, T, _fluid_enthalpy, _dummy_var);
      Real enthalpy_water = (_fluid_enthalpy / 1000);

      return -_test[_i][_qp]*_fluid_enthalpy;
    }

  else if (_return_parameter == 3 )
    {
      return -_test[_i][_qp]*T;
    }

  else if ( _return_parameter == 4 )
    {
      return -_test[_i][_qp]*P;
    }
  else
    {
      return 0;
    }
}


  
  
  

  
  
  

  

  

  
  

  

  
  


















