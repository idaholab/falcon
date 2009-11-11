#include "DarcyWater.h"
#include "SteamTables.h"

//Function to calc water density, single phase conditions only
double density_(double T)
{
  double _rho_w;
  _rho_w=1000.*(1-((pow(((T)-3.9863),2)/508929.2)*(((T)+288.9414)/((T)+68.12963))));
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
InputParameters valid_params<DarcyWater>()
{
  InputParameters params;
  params.set<Real>("permeability")=1.0E-12; //intrinsic permeability, "k", in (m^2)
  params.set<Real>("porosity")=0.10; //dimensionless but variable
  params.set<Real>("rho_w")=1000.0; //water density, variable, in (kg/m^3)
  params.set<Real>("mu_w")=0.001; //water dynamic viscosity, variable, in (Pa s)
  params.set<Real>("c_f")=1.0;//"consolodation factor", use for modifing porosity from
                              // geomechanics
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

DarcyWater::DarcyWater(std::string name,
                                 InputParameters parameters,
                                 unsigned int block_id,
                                 std::vector<std::string> coupled_to,
                                 std::vector<std::string> coupled_as)
  
  :Material(name,parameters,block_id,coupled_to,coupled_as),
   _input_permeability(parameters.get<Real>("permeability")),
   _input_porosity(parameters.get<Real>("porosity")),
   _input_rho_w(parameters.get<Real>("rho_w")),
   _input_mu_w(parameters.get<Real>("mu_w")),
   _input_c_f(parameters.get<Real>("c_f")),
   _input_thermal_conductivity(parameters.get<Real>("thermal_conductivity")),
   _input_time_coefficient(parameters.get<Real>("time_coefficient")),
   _input_water_specif_heat(parameters.get<Real>("water_specific_heat")),
   _input_rock_specific_heat(parameters.get<Real>("rock_specific_heat")),
   _input_rho_r(parameters.get<Real>("rho_r")),
   _input_gravity(parameters.get<Real>("gravity")),
   
   _gx(parameters.get<Real>("gx")),
   _gy(parameters.get<Real>("gy")),
   _gz(parameters.get<Real>("gz")),

   _permeability(declareRealProperty("permeability")),
   _porosity(declareRealProperty("porosity")),
   _rho_w(declareRealProperty("rho_w")),
   _mu_w(declareRealProperty("mu_w")),
   _c_f(declareRealProperty("c_f")),
   _thermal_conductivity(declareRealProperty("thermal_conductivity")),
   _time_coefficient(declareRealProperty("time_coefficient")),
   _gravity(declareRealProperty("gravity")),
   _water_specific_heat(declareRealProperty("water_specific_heat")),
   _rock_specific_heat(declareRealProperty("rock_specific_heat")),
   _rho_r(declareRealProperty("rho_r")),

   _darcy_params(declareRealProperty("darcy_params")),
   
   //  _darcy_flux(declareGradientProperty("darcy_pressure")),
   _darcy_flux(declareGradientProperty("darcy_flux")),
   _darcy_velocity(declareGradientProperty("darcy_velocity")),
   _gravity_vector(declareRealVectorValueProperty("gravity_vector")),
   _grad_p(coupledGrad("pressure")),
   _pressure(coupledVal("pressure")),
   _temperature(coupledVal("temperature")),
   _grad_T(coupledGrad("temperature"))
{}

void
DarcyWater::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    _gravity_vector[qp](0) = _gx;
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    
    _permeability[qp] = _input_permeability;
    _porosity[qp] = _input_porosity;
    _rho_w[qp] = _input_rho_w;
    _mu_w[qp] = _input_mu_w;
    _thermal_conductivity[qp] = _input_thermal_conductivity;
    _time_coefficient[qp] = _input_time_coefficient;
    _gravity[qp] = _input_gravity;
    
    //SteamTables::steam_call_(_pressure[qp], _temperature[qp], _rho_w[qp], _mu_w[qp]);
    
    //RKP:  Function call to "density_" to calc rho_w
    //  
    _rho_w[qp] = density_((_temperature)[qp]);
    //RKP:  Function call to "viscosity_" to calc mu_w
    //
    _mu_w[qp] = viscosity_((_temperature)[qp]);

    _darcy_params[qp] = ((_permeability[qp] * _rho_w[qp]) / _mu_w[qp]);
    _darcy_flux[qp] = -((_permeability)[qp] / (_mu_w)[qp]) * ((_grad_p[qp])+((_rho_w)[qp]*(_gravity)[qp]*(_gravity_vector)[qp]));
    _darcy_velocity[qp] = (_darcy_flux[qp]) / _porosity[qp];
    
    /*
    std::cerr << "Pressure  =   " <<(_pressure)[qp]<<"   Temp =  "<<(_temperature)[qp]<<"\n";
    //std::cerr << (_mu_w)[qp] <<" * "<< (_rho_w)[qp] <<" * "<< (_permeability)[qp] <<" = "<< (_darcy_params)[qp]<<"\n";
    //std::cerr << "Darcy Flux Vector =  " << (_darcy_flux)[qp];//<<"\n";
    //std::cerr << "gravity Vector =  " << (_gravity_vector)[qp];//<<"\n";
    std::cerr << "Point =   " <<_q_point[qp]<<"\n";
    
    std::cerr << "Darcy Velocity =   " <<_darcy_velocity[qp]<<"\n";
    // std::cerr << "Pressure  =   " <<(_pressure)[qp]<<"   Temp =  "<<(_temperature)[qp]<<"\n";
    //std::cerr << "grad Pressure  =   " <<(_grad_p)[qp];
    //std::cerr << "grad Temp  =   " <<(_grad_T)[qp];
    //std::cerr << "density  =   " <<(_rho_w)[qp]<< "     viscosity  =   " <<(_mu_w)[qp]<<"\n";
    //std::cerr << "viscosity  =   " <<(_mu_w)[qp]<<"\n";
    //std::cerr <<"\n";
    */
  }
  
}



  
