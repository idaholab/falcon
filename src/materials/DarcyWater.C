#include "DarcyWater.h"
// #include "SteamTables.h"

//Function to calc water density, single phase conditions only
double density_(double T)
{
  double a;
  a = 1000.*(1-((pow(((T)-3.9863),2)/508929.2)*(((T)+288.9414)/((T)+68.12963))));
   return (a);
  double _density_water;
  _density_water=1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
   return (_density_water);
}
//end density function


//Function call to calc viscosity, can define local variables here....need to finish 11-6-09
double viscosity_(double T)
{
  double _viscosity_water, a, b, c, d;
  
  if (T < 0.)
    {
      std::cerr << "T= " << T ;
      mooseError("Temperature out of Range");
    }
    
    else if (T <= 40.)
    {
      a = 1.787E-3;
      b = (-0.03288+(1.962E-4*T))*T;
      _viscosity_water = a * exp(b);
     }
    
    else if (T <= 100.)
    {
      a = 1e-3;
      b = (1+(0.015512*(T-20)));
      c = -1.572;
      _viscosity_water = a * pow(b,c);
    }
    
    else if (T <= 300.)
    {
      a = 0.2414;
      b = 247 / (T+133.15);
      c = (a * pow(10,b));
      _viscosity_water = c * 1E-4;
     }
    
    else
    {
      std::cerr << "T= " << T;
      mooseError("Temperature out of Range");
    }
  return (_viscosity_water);
}
//end viscosity function


template<>
InputParameters validParams<DarcyWater>()
{
  InputParameters params = validParams<Material>();
  params.set<Real>("permeability")=1.0E-12; //intrinsic permeability, "k", in (m^2)
  params.set<Real>("porosity")=0.10; //dimensionless but variable
  params.set<Real>("density_water")=1000.0; //water density, variable, in (kg/m^3)
  params.set<Real>("viscosity_water")=0.001; //water dynamic viscosity, variable, in (Pa s)
  params.set<Real>("compressibility")=1.0;//"fluid compressibility", use for modifing
  params.set<Real>("thermal_conductivity") = 1.0;  //thermal thermal_conductivity, in (m^2/s)
  params.set<Real>("time_coefficient") = 1.0; //leftover from example, carry it along for now
  params.set<Real>("gravity") = 9.80665; //gravity acceleration, in (m/s^2)
  params.set<Real>("specific_heat_water") = 4186.0;  //units of (J/(kg K))
  params.set<Real>("specific_heat_rock") = 1.00e3;  //units of (J/(kg K))
  params.set<Real>("density_rock") = 2.50e3;  //rock density, in  (kg/m^3)

  params.set<Real>("gx")= 0.0;  //x component of the gravity pressure vector
  params.set<Real>("gy")= 0.0; //y component of the gravity pressure vector
  params.set<Real>("gz")= 1.0;  //z component of the gravity pressure vector

  //  params.set<RealGradient>("darcy_velocity")=0.0;
  

return params;
}

DarcyWater::DarcyWater(const std::string & name,
                       InputParameters parameters)
  :Material(name, parameters),
   _input_permeability(getParam<Real>("permeability")),
   _input_porosity(getParam<Real>("porosity")),
   _input_density_water(getParam<Real>("density_water")),
   _input_viscosity_water(getParam<Real>("viscosity_water")),
   _input_compressibility(getParam<Real>("compressibility")),
   _input_thermal_conductivity(getParam<Real>("thermal_conductivity")),
   _input_time_coefficient(getParam<Real>("time_coefficient")),
   _input_water_specif_heat(getParam<Real>("specific_heat_water")),
   _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
   _input_density_rock(getParam<Real>("density_rock")),
   _input_gravity(getParam<Real>("gravity")),
   
   _gx(getParam<Real>("gx")),
   _gy(getParam<Real>("gy")),
   _gz(getParam<Real>("gz")),

   _permeability(declareProperty<Real>("permeability")),
   _porosity(declareProperty<Real>("porosity")),
   _density_water(declareProperty<Real>("density_water")),
   _viscosity_water(declareProperty<Real>("viscosity_water")),
   _compressibility(declareProperty<Real>("compressibility")),
   _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
   _time_coefficient(declareProperty<Real>("time_coefficient")),
   _gravity(declareProperty<Real>("gravity")),
   _specific_heat_water(declareProperty<Real>("specific_heat_water")),
   _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
   _density_rock(declareProperty<Real>("density_rock")),

   _tau_water(declareProperty<Real>("tau_water")),
   
   //  _darcy_flux(declareGradientProperty("darcy_pressure")),
   _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
   _darcy_velocity_w(declareProperty<RealGradient>("darcy_velocity_w")),
   _gravity_vector(declareProperty<RealVectorValue>("gravity_vector")),
   _grad_p(coupledGradient("pressure")),
   _pressure(coupledValue("pressure")),
   _temperature(coupledValue("temperature")),
   _grad_T(coupledGradient("temperature"))
{}

void
DarcyWater::computeProperties()
{
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {
    _gravity_vector[qp](0) = _gx;
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    
    _permeability[qp] = _input_permeability;
    _compressibility[qp] = _input_compressibility; //Rob missed this
    _porosity[qp] = _input_porosity;
    _density_water[qp] = _input_density_water;
    _viscosity_water[qp] = _input_viscosity_water;
    _thermal_conductivity[qp] = _input_thermal_conductivity;
    _time_coefficient[qp] = _input_time_coefficient;
    _gravity[qp] = _input_gravity;
    _density_rock[qp]   = _input_density_rock;
    
    
    //SteamTables::steam_call_(_pressure[qp], _temperature[qp], _density_water[qp], _viscosity_water[qp]);
    
    //RKP:  Function call to "density_" to calc density_water
    //  
     _density_water[qp] = density_((_temperature)[qp]);

     //RKP:  Function call to "viscosity_" to calc viscosity_water
    //
    // _viscosity_water[qp] = viscosity_((_temperature)[qp]);

    _tau_water[qp] = ((_permeability[qp] * _density_water[qp]) / _viscosity_water[qp]);
    _darcy_flux_water[qp] = -((_permeability)[qp] / (_viscosity_water)[qp]) * ((_grad_p[qp])+((_density_water)[qp]*(_gravity)[qp]*(_gravity_vector)[qp]));
    _darcy_velocity_w[qp] = (_darcy_flux_water[qp]) / _porosity[qp];
    
    
    //std::cerr << "Pressure  =   " <<(_pressure)[qp]<<"   Temp =  "<<(_temperature)[qp]<<"\n";
    //std::cerr << (_viscosity_water)[qp] <<" * "<< (_density_water)[qp] <<" * "<< (_permeability)[qp] <<" = "<< (_darcy_params)[qp]<<"\n";
    //std::cerr << "Darcy Flux Vector =  " << (_darcy_flux)[qp];//<<"\n";
    //std::cerr << "gravity Vector =  " << (_gravity_vector)[qp];//<<"\n";
    //std::cerr << "Point =   " <<_q_point[qp]<<"\n";
    
    //std::cerr << "Darcy Velocity =   " <<_darcy_velocity[qp]<<"\n";
    // std::cerr << "Pressure  =   " <<(_pressure)[qp]<<"   Temp =  "<<(_temperature)[qp]<<"\n";
    //std::cerr << "grad Pressure  =   " <<(_grad_p)[qp];
    //std::cerr << "grad Temp  =   " <<(_grad_T)[qp];
    //std::cerr << "density  =   " <<(_density_water)[qp]<< "     viscosity  =   " <<(_viscosity_water)[qp]<<"\n";
//    std::cerr << "rock density  =   " << _density_rock[qp]<<"\n";
//  std::cerr << "rock specific heat  =   " << _specific_heat_rock[qp]<<"\n";
    //std::cerr <<"\n";
    
  }
  
}



  
