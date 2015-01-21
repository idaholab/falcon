#include "WellUserObject.h"

template<>
InputParameters validParams<WellUserObject>()
{
  InputParameters params = validParams<UserObject>();
  return params;
}

WellUserObject::WellUserObject(const std::string & name, InputParameters params) :
  GeneralUserObject(name, params)
{ }

WellUserObject::~WellUserObject()
{ }

Real WellUserObject::WellBottomTemperature ( Real _well_depth, Real _surface_earth_temperature, Real _surface_fluid_density, Real _fluid_heat_capacity, Real _thermal_conductivity_earth, Real _thermal_conductivity_casing, Real _thermal_conductivity_cement, Real _thermal_conductivity_water, Real _well_radius, Real _casing_thickness, Real _cement_thickness, Real _reservoir_thickness, Real _reservoir_thermal_gradient, Real _return_depth_T_P, Real _mass_flow_rate, Real _surface_fluid_temperature, Real& T ) const
{

  //Calculate the maximum velocity from the mass flow rate
  double _velocity_max = ( _mass_flow_rate / _surface_fluid_density ) / ( libMesh::pi*_well_radius*_well_radius );


  //Calculate convective heat transfer coefficient (water)
  double _convective_heat_transfer = ( 4*_thermal_conductivity_water ) / _well_radius;

  //Calculate overall heat transfer coefficient U
  double U;

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
  double _delta_r = _well_radius / _num_points_r;

  double _delta_z = 0.1;
  int _num_points_z = _well_depth / _delta_z;
  int _point_return_depth = _return_depth_T_P / _delta_z;

  double _r[_num_points_r];
  double _z[_num_points_z];

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
  double T_aea[_num_points_z][_num_points_r];
  double T_edge[_num_points_z];
  double q;

  //Calculate q and T
  for(int nRow = 0; nRow < _num_points_z; nRow++)
    {
      //calculate temperature of the earth T_e for depth z
      double _temperature_earth = _surface_earth_temperature + _reservoir_thermal_gradient*_z[nRow];

      //calculate q for particular depth
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

  T = T_edge[_point_return_depth-1];
  return T;
}

Real WellUserObject::WellBottomPressure (Real _well_depth, Real _surface_fluid_density, Real _well_radius, Real _return_depth_T_P, Real _mass_flow_rate, Real _surface_fluid_viscosity, Real _well_head_pressure, Real _gravity, Real _surface_roughness, Real& P) const
{

  //calculate diameter of wellbore
  double _diameter = 2 * _well_radius;

  //calculate fluid velocity from mass flow rate
  double _velocity = ( _mass_flow_rate / _surface_fluid_density ) / ( libMesh::pi*_well_radius*_well_radius );

  //calculate the reynold's number
  double Re = ( _diameter*_velocity*_surface_fluid_density) / _surface_fluid_viscosity;
  double f;

   if (Re == 0)
    {
      f = 0;
    }
  else if (Re >= 4000)
    {
      f = 0.001375*(1+std::pow((20000*(_surface_roughness/_diameter)+1000000/Re),1/3.));
    }
  else
    {
      f = 16 / Re;
    }

  //Calculate friction
   double F = 4*f* (( _return_depth_T_P ) / _diameter) * ((_velocity*_velocity) / 2);

  //calculate P
  P = _well_head_pressure + _surface_fluid_density*_gravity*_return_depth_T_P - _surface_fluid_density*F;
  return P;
}
