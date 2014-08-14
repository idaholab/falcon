#include "TemperatureFunction.h"

template<>
InputParameters validParams<TemperatureFunction>()
{
    InputParameters params = validParams<Function>();
    params.addParam<Real>("surface_earth_temperature", 315, "surface earth temperature, [K]");
    params.addParam<Real>("surface_fluid_density", 992.2, "density of fluid, [kg/m^3]");
    params.addParam<Real>("fluid_heat_capacity", 4185, "heat capacity of fluid, [J/(kg.K)]");
    params.addParam<Real>("thermal_conductivity_earth", 1.5, "thermal conductivity of earth, [W/(m.K)]");
    params.addParam<Real>("thermal_conductivity_casing", 43, "thermal conductivity of casing [W/(m.K)]");
    params.addParam<Real>("thermal_conductivity_cement", 1.4, "thermal conductivity of well cement [W/(m.K)]");
    params.addParam<Real>("thermal_conductivity_water", 0.58, "thermal conductivity of water [W/(m.K)]");
    params.addParam<Real>("well_radius", 0.127, "radius of the well [m]");
    params.addParam<Real>("well_depth", 2000, "depth of the well [m]");
    params.addParam<Real>("casing_thickness", 0.038, "thickness of the casing [m] (enter zero if no casing)");
    params.addParam<Real>("cement_thickness", 0.051, "thickness of the cement [m] (enter zero if no cement)");
    params.addParam<Real>("reservoir_thickness", 100, "thickness of the reservoir [m]");
    params.addParam<Real>("reservoir_thermal_gradient", 0.02, "thermal gradient of the reservoir [K/m]");
    params.addParam<Real>("temp_return_depth", 1800, "depth to calculate temperature [m]");
    params.addParam<std::vector<Real> >("schedule", std::vector<Real>(0.0), "The times each injection stage starts");
    params.addParam<std::vector<Real> >("mass_flow_rate", std::vector<Real>(0.0), "The injection range at each stage");
    params.addParam<std::vector<Real> >("surface_fluid_temperature", std::vector<Real>(0.0), "The temperature of the injection at each stage"); 
    return params;
}

TemperatureFunction::TemperatureFunction(const std::string & name, InputParameters parameters):
    Function(name, parameters),
    _surface_earth_temperature(getParam<Real>("surface_earth_temperature")),
    _surface_fluid_density(getParam<Real>("surface_fluid_density")),
    _fluid_heat_capacity(getParam<Real>("fluid_heat_capacity")),
    _thermal_conductivity_earth(getParam<Real>("thermal_conductivity_earth")),
    _thermal_conductivity_casing(getParam<Real>("thermal_conductivity_casing")),
    _thermal_conductivity_cement(getParam<Real>("thermal_conductivity_cement")),
    _thermal_conductivity_water(getParam<Real>("thermal_conductivity_water")),
    _well_radius(getParam<Real>("well_radius")),
    _well_depth(getParam<Real>("well_depth")),
    _casing_thickness(getParam<Real>("casing_thickness")),
    _cement_thickness(getParam<Real>("cement_thickness")),
    _reservoir_thickness(getParam<Real>("reservoir_thickness")),
    _reservoir_thermal_gradient(getParam<Real>("reservoir_thermal_gradient")),
    _temp_return_depth(getParam<Real>("temp_return_depth")),
    _schedule(getParam<std::vector<Real> >("schedule")), 
    mass_flow_rate(getParam<std::vector<Real> >("mass_flow_rate")), 
    surface_fluid_temperature(getParam<std::vector<Real> >("surface_fluid_temperature"))
  
{}

Real
TemperatureFunction::value(Real /*t*/, const Point & p)
{
  int _nstages = _schedule.size();
  int _size_temp_array = surface_fluid_temperature.size();
  int _size_mass_flow_array = mass_flow_rate.size();

  Real _mass_flow_rate;
  Real _surface_fluid_temperature;

  if (_nstages > 1)
    {
      for (unsigned int i=0; i < _nstages; i++)
	{
	  if(_t >= _schedule[i] and _t < _schedule[i+1])
	    {
	      _mass_flow_rate = mass_flow_rate[i];
	      _surface_fluid_temperature = surface_fluid_temperature[i];
	      break;
	    }
	}

      if (_t >= _schedule[_nstages - 1])
	    {
	      _mass_flow_rate = mass_flow_rate[_nstages -1];
	      _surface_fluid_temperature = surface_fluid_temperature[_nstages -1];
	    }

    
      else if ( _t <= _schedule[0] )
           {
              _mass_flow_rate = mass_flow_rate[0];
              _surface_fluid_temperature = surface_fluid_temperature[0];
           }
    }   

    //Calculate the maximum velocity from the mass flow rate
    double _velocity_max = ( _mass_flow_rate / _surface_fluid_density ) / ( libMesh::pi*_well_radius*_well_radius );

    //Calculate convective heat transfer coefficient (water)
    Real _convective_heat_transfer = ( 4*_thermal_conductivity_water ) / _well_radius;
    Real U;

    //Calculate overall heat transfer coefficient U
    if(_casing_thickness == 0 && _cement_thickness == 0)
    {
        U = 1 / ( (1/_convective_heat_transfer) + (_well_radius/_thermal_conductivity_earth)*log((_well_radius+_casing_thickness+_cement_thickness+_reservoir_thickness)/_well_radius));
    }
    else if(_casing_thickness != 0 && _cement_thickness == 0)
    {
        U = 1 / ( (1/_convective_heat_transfer) + (_well_radius/_thermal_conductivity_casing) * log((_well_radius+_casing_thickness)/_well_radius)  + (_well_radius/_thermal_conductivity_earth)*log((_well_radius+_casing_thickness+_reservoir_thickness)/(_well_radius+_casing_thickness)));
    }
    else if(_casing_thickness != 0 && _cement_thickness != 0)
    {
        U = 1 / ( (1/_convective_heat_transfer) + (_well_radius/_thermal_conductivity_casing) * log((_well_radius+_casing_thickness)/_well_radius) + (_well_radius/_thermal_conductivity_cement)*log((_well_radius+_casing_thickness+_cement_thickness)/(_well_radius+_casing_thickness)) + (_well_radius/_thermal_conductivity_earth)*log((_well_radius+_casing_thickness+_cement_thickness+_reservoir_thickness)/(_well_radius+_casing_thickness+_cement_thickness)));
    }
    else if(_casing_thickness == 0 && _cement_thickness != 0)
    {
        U = 1 / ( (1/_convective_heat_transfer) + (_well_radius/_thermal_conductivity_cement)*log((_well_radius+_cement_thickness)/(_well_radius)) + (_well_radius)/_thermal_conductivity_earth)*log((_well_radius+_cement_thickness+_reservoir_thickness)/(_well_radius+_cement_thickness));
    }

    //Determine points r, z for analytical calculation.
    int _num_points_r = 20;
    Real _delta_r = _well_radius / _num_points_r;


    Real _delta_z = 0.1;
    int _num_points_z = _well_depth / _delta_z;
    int _point_return_depth = _temp_return_depth / _delta_z;


    Real _r[_num_points_r];
    Real _z[_num_points_z];

    for(int i = 0; i < _num_points_r; i++)
    {
        if( i == 0 )
            {
            _r[i] = _delta_r;
            }
        else
            {
            _r[i] = _r[i-1] + _delta_r;
	    }
    }

    for(int j = 0; j < _num_points_z; j++)
    {
        if( j == 0 )
            {
            _z[j] = _delta_z;
            }
        else
            {
            _z[j] = _z[j-1] + _delta_z;
            }
    }

    //create matrix to store temp.
    Real T_aea[_num_points_z][_num_points_r];
    Real T_edge[_num_points_z];
    Real q;
   
    //Calculate q and T
    for(int nRow = 0; nRow < _num_points_z; nRow++)
    {

        //calculate temperature of the earth T_e for depth z
        Real _temperature_earth = _surface_earth_temperature + _reservoir_thermal_gradient*_z[nRow];

        //calculate q
        if( nRow == 0 )
	  {
            q = U*(_temperature_earth - _surface_fluid_temperature);
	  }
        else
	  {
            q = U*(_temperature_earth - T_aea[nRow - 1][0]);   
          }

        for(int nCol = 0; nCol < _num_points_r; nCol++)
        {

	  if(_velocity_max == 0) //no convecton, conduction heating only
	    {
	      T_aea[nRow][nCol] = _surface_fluid_temperature - ( ( q*_well_radius ) / ( 4*_thermal_conductivity_earth ) ) * ( (_r[nCol]/_well_radius)*(_r[nCol]/_well_radius)*(_r[nCol]/_well_radius)*(_r[nCol]/_well_radius) - 4*(_r[nCol]/_well_radius)*(_r[nCol]/_well_radius) + 7/6 );

	       if( nCol == _num_points_r-1 )
		{
		  T_edge[nRow] = T_aea[nRow][nCol];
       		} 
	     }
	  else
	    {
	      T_aea[nRow][nCol] = _surface_fluid_temperature + ( (4*q*_z[nRow]) / ( _surface_fluid_density*_fluid_heat_capacity*_velocity_max*_well_radius) ) - ( ( q*_well_radius ) / ( 4*_thermal_conductivity_earth ) ) * ( (_r[nCol]/_well_radius)*(_r[nCol]/_well_radius)*(_r[nCol]/_well_radius)*(_r[nCol]/_well_radius) - 4*(_r[nCol]/_well_radius)*(_r[nCol]/_well_radius) +7/6 );
	      if( nCol == _num_points_r-1 )
		{
		  T_edge[nRow] = T_aea[nRow][nCol];
      		}
	    }
	}
    }
   
    return T_edge[_point_return_depth-1];
}
