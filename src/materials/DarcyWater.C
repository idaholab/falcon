#include "DarcyWater.h"
// #include "SteamTables.h"

//Function to calc water density, single phase conditions only
double density_(double T)
{
  double a;
  a = 1000.*(1-((pow(((T)-3.9863),2)/508929.2)*(((T)+288.9414)/((T)+68.12963))));
   return (a);
  double _rho_w;
  _rho_w=1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
   return (_rho_w);
}
//end density function


//Function call to calc viscosity, can define local variables here....need to finish 11-6-09
double viscosity_(double T)
{
  double _mu_w, a, b, c, d;
  
  if (T < 0.)
    {
      std::cerr << "T= " << T ;
      mooseError("Temperature out of Range");
    }
    
    else if (T <= 40.)
    {
      a = 1.787E-3;
      b = (-0.03288+(1.962E-4*T))*T;
      _mu_w = a * exp(b);
     }
    
    else if (T <= 100.)
    {
      a = 1e-3;
      b = (1+(0.015512*(T-20)));
      c = -1.572;
      _mu_w = a * pow(b,c);
    }
    
    else if (T <= 300.)
    {
      a = 0.2414;
      b = 247 / (T+133.15);
      c = (a * pow(10,b));
      _mu_w = c * 1E-4;
     }
    
    else
    {
      std::cerr << "T= " << T;
      mooseError("Temperature out of Range");
    }
  return (_mu_w);
}
//end viscosity function


template<>
InputParameters validParams<DarcyWater>()
{
  InputParameters params = validParams<Material>();
  params.set<Real>("permeability")=1.0E-12; //intrinsic permeability, "k", in (m^2)
  params.set<Real>("porosity")=0.10; //dimensionless but variable
  params.set<Real>("rho_w")=1000.0; //water density, variable, in (kg/m^3)
  params.set<Real>("mu_w")=0.001; //water dynamic viscosity, variable, in (Pa s)
  params.set<Real>("c_f")=1.0;//"fluid compressibility", use for modifing
  params.set<Real>("thermal_conductivity") = 1.0;  //thermal thermal_conductivity, in (m^2/s)
  params.set<Real>("time_coefficient") = 1.0; //leftover from example, carry it along for now
  params.set<Real>("gravity") = 9.80665; //gravity acceleration, in (m/s^2)
  params.set<Real>("water_specific_heat") = 4186.0;  //units of (J/(kg K))
  params.set<Real>("rock_specific_heat") = 1.00e3;  //units of (J/(kg K))
  params.set<Real>("rho_r") = 2.50e3;  //rock density, in  (kg/m^3)

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
   _input_rho_w(getParam<Real>("rho_w")),
   _input_mu_w(getParam<Real>("mu_w")),
   _input_c_f(getParam<Real>("c_f")),
   _input_thermal_conductivity(getParam<Real>("thermal_conductivity")),
   _input_time_coefficient(getParam<Real>("time_coefficient")),
   _input_water_specif_heat(getParam<Real>("water_specific_heat")),
   _input_rock_specific_heat(getParam<Real>("rock_specific_heat")),
   _input_rho_r(getParam<Real>("rho_r")),
   _input_gravity(getParam<Real>("gravity")),
   
   _gx(getParam<Real>("gx")),
   _gy(getParam<Real>("gy")),
   _gz(getParam<Real>("gz")),

   _permeability(declareProperty<Real>("permeability")),
   _porosity(declareProperty<Real>("porosity")),
   _rho_w(declareProperty<Real>("rho_w")),
   _mu_w(declareProperty<Real>("mu_w")),
   _c_f(declareProperty<Real>("c_f")),
   _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
   _time_coefficient(declareProperty<Real>("time_coefficient")),
   _gravity(declareProperty<Real>("gravity")),
   _water_specific_heat(declareProperty<Real>("water_specific_heat")),
   _rock_specific_heat(declareProperty<Real>("rock_specific_heat")),
   _rho_r(declareProperty<Real>("rho_r")),

   _darcy_params_w(declareProperty<Real>("darcy_params_w")),
   
   //  _darcy_flux(declareGradientProperty("darcy_pressure")),
   _darcy_flux_w(declareProperty<RealGradient>("darcy_flux_w")),
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
    _c_f[qp] = _input_c_f; //Rob missed this
    _porosity[qp] = _input_porosity;
    _rho_w[qp] = _input_rho_w;
    _mu_w[qp] = _input_mu_w;
    _thermal_conductivity[qp] = _input_thermal_conductivity;
    _time_coefficient[qp] = _input_time_coefficient;
    _gravity[qp] = _input_gravity;
    _rho_r[qp]   = _input_rho_r;
    
    
    //SteamTables::steam_call_(_pressure[qp], _temperature[qp], _rho_w[qp], _mu_w[qp]);
    
    //RKP:  Function call to "density_" to calc rho_w
    //  
     _rho_w[qp] = density_((_temperature)[qp]);

     //RKP:  Function call to "viscosity_" to calc mu_w
    //
    // _mu_w[qp] = viscosity_((_temperature)[qp]);

    _darcy_params_w[qp] = ((_permeability[qp] * _rho_w[qp]) / _mu_w[qp]);
    _darcy_flux_w[qp] = -((_permeability)[qp] / (_mu_w)[qp]) * ((_grad_p[qp])+((_rho_w)[qp]*(_gravity)[qp]*(_gravity_vector)[qp]));
    _darcy_velocity_w[qp] = (_darcy_flux_w[qp]) / _porosity[qp];
    
    
    //std::cerr << "Pressure  =   " <<(_pressure)[qp]<<"   Temp =  "<<(_temperature)[qp]<<"\n";
    //std::cerr << (_mu_w)[qp] <<" * "<< (_rho_w)[qp] <<" * "<< (_permeability)[qp] <<" = "<< (_darcy_params)[qp]<<"\n";
    //std::cerr << "Darcy Flux Vector =  " << (_darcy_flux)[qp];//<<"\n";
    //std::cerr << "gravity Vector =  " << (_gravity_vector)[qp];//<<"\n";
    //std::cerr << "Point =   " <<_q_point[qp]<<"\n";
    
    //std::cerr << "Darcy Velocity =   " <<_darcy_velocity[qp]<<"\n";
    // std::cerr << "Pressure  =   " <<(_pressure)[qp]<<"   Temp =  "<<(_temperature)[qp]<<"\n";
    //std::cerr << "grad Pressure  =   " <<(_grad_p)[qp];
    //std::cerr << "grad Temp  =   " <<(_grad_T)[qp];
    //std::cerr << "density  =   " <<(_rho_w)[qp]<< "     viscosity  =   " <<(_mu_w)[qp]<<"\n";
//    std::cerr << "rock density  =   " << _rho_r[qp]<<"\n";
//  std::cerr << "rock specific heat  =   " << _rock_specific_heat[qp]<<"\n";
    //std::cerr <<"\n";
    
  }
  
}



  
