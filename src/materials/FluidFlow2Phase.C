#include "FluidFlow2Phase.h"

template<>
InputParameters validParams<FluidFlow2Phase>()
{
  InputParameters params = validParams<PorousMedia>(); //inherit porous medium basic properties
  params.addParam<Real>("density_water",1000.0,"fluid density in Kg/m^3");
  params.addParam<Real>("density_steam",0.01,"steam density in Kg/m^3");
  params.addParam<Real>("compressibility", 4.6e-10,"fluid compressibility in 1/Pa");
  params.addParam<Real>("viscosity_water",0.001,"fluid dynamic viscosity in Pa.s");
  params.addParam<Real>("viscosity_steam",0.00001,"steam dynamic viscosity in Pa.s");
  params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("enthalpy", "TODO: add description");
  params.addCoupledVar("tempAux", "TODO: add description");
     
  return params;
}

FluidFlow2Phase::FluidFlow2Phase(const std::string & name,
                                 InputParameters parameters)
  :PorousMedia(name, parameters),

   _grad_p(coupledGradient("pressure")),
   _pressure(coupledValue("pressure")),

   //should this be "pressure_old" ?
   _pressure_old(coupledValueOld("pressure")),
   _enthalpy(coupledValue("enthalpy")),
   _enthalpy_old(coupledValueOld("enthalpy")),

   
   _input_density_water(getParam<Real>("density_water")),
   _input_density_steam(getParam<Real>("density_steam")),
   _input_compressibility(getParam<Real>("compressibility")),
   _input_viscosity_water(getParam<Real>("viscosity_water")),
   _input_viscosity_steam(getParam<Real>("viscosity_steam")),

   
   //delcare material properties

   _density_water(declareProperty<Real>("density_water")),
   _density_steam(declareProperty<Real>("density_steam")),
   _compressibility(declareProperty<Real>("compressibility")),
   _viscosity_water(declareProperty<Real>("viscosity_water")),
   _viscosity_steam(declareProperty<Real>("viscosity_steam")),
   _rel_perm_water(declareProperty<Real>("rel_perm_water")),
   _rel_perm_steam(declareProperty<Real>("rel_perm_steam")),

   _temp(declareProperty<Real>("temperature")),
   _temp_old(declareProperty<Real>("temperature_old")),
   _dTbydP_H(declareProperty<Real>("dTbydP_H")),
   _dTbydH_P(declareProperty<Real>("dTbydH_P")),
     
   _density(declareProperty<Real>("density")),
   _density_old(declareProperty<Real>("density_old")),
   _ddensitybydP_H(declareProperty<Real>("ddensitybydP_H")),
   _ddensitybydH_P(declareProperty<Real>("ddensitybydH_P")),
   
   _S_water(declareProperty<Real>("S_water")),
   _S_steam(declareProperty<Real>("S_steam")),
   _enthalpy_saturated_water(declareProperty<Real>("enthalpy_saturated_water")),
   _enthalpy_saturated_steam(declareProperty<Real>("enthalpy_saturated_steam")),
   _Genthalpy_saturated_water(declareProperty<RealGradient>("grad_enthalpy_saturated_water")),
   _Genthalpy_saturated_steam(declareProperty<RealGradient>("grad_enthalpy_saturated_steam")),
   _tau_water(declareProperty<Real>("tau_water")),
   _tau_steam(declareProperty<Real>("tau_steam")),
   _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
   _darcy_flux_steam(declareProperty<RealGradient>("darcy_flux_steam"))
     

{
  E3 = 1e3;
  E6 = 1e6;
  E7 = 1e-7;

// coefficients related to density_water empirical equation     
  a1 = 0.989875;
  a2 = 4.00894e-4;
  a3 = 4.00489e-5;
  a4 = 2.66608;
  a5 = 5.46283e-7;
  a6 = 1.29958e-7;

// coefficients related to density_steam empirical equation
  b1 = 2.26162e-5;
  b2 = 0.0438441;
  b3 = 1.79088e-5;
  b4 = 3.69276e-8;
  b5 = 5.17644e-13;

// coefficients related to enthalpy_saturated_steam empirical equation     
  c1 = 2822.82;
  c2 = 39.952;
  c3 = 2.54342;
  c4 = 0.938879;

// coefficients related to enthalpy_saturated_water empirical equation     
  d1 = 809.674;
  d2 = 94.465;
  d3 = 4.502;
  d4 = 0.120265;
  d5 = 162.7;
  d6 = 29.8163;
  d7 = 1.75623;

}

void
FluidFlow2Phase::computeProperties()
{
  PorousMedia::computeProperties();

    
  
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {
  
//   fluid properties
    _density_water[qp]               = _input_density_water;
    _viscosity_water[qp]                = _input_viscosity_water;
    _density_steam[qp]               = _input_density_steam;
    _viscosity_steam[qp]                = _input_viscosity_steam;

//   For two phase ie water and steam
//   we are using empirical equations to get thermodynamic data which are in Mpa and j/g......
//   we need to convert pressure from pa to Mpa and Enthalpy from J/kg to J/g
       
    Real H = _enthalpy[qp]/E3;
    Real P = _pressure[qp]/E6;
    Real P2 = pow(P,2);  
    Real P3 = pow(P,3);
    Real P4 = pow(P,4);
    Real H2 = pow(H,2);
    Real H3 = pow(H,3);
    double a;
    double b;
    double c;
    Real _ddensity_waterbydP_H = 1;
    Real _ddensity_waterbydH_P = 1;
    Real _ddensity_steambydP_H = 1;
    Real _ddensity_steambydH_P = 1;

     
    _enthalpy_saturated_steam[qp]  = c1-(c2/P)+(c3/P2)-(c4*P2);
    _enthalpy_saturated_water[qp]  = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);

//   _Genthalpy_saturated_steam[qp] = _grad_p[qp]/E3*((c2/P2)-(c3*2/P3)-(c4*2*P));  
//   _Genthalpy_saturated_water[qp] = _grad_p[qp]/E3*((d2)-(d3*2*P)+(d4*3*P2)+(d5/P2)-(d6*2/P3)+(d7*3/P4));
     

     
//   ***********compressed water zone*************
//   ********************************************
    if (H < _enthalpy_saturated_water[qp])
    {
      if (H >200.0)
      {
        _density_water[qp] = E3*(a1+(a2*P)-(a3*H)+(a4/H)+(a5*P*H)-(a6*H2));
         
//   _ddensity_waterbydP_H = ((a2)+(a5*H))/E3;
//   _ddensity_waterbydH_P = -(a3)-(a4/H2)+(a5*P)-(2*a6*H);
      }
      else
      {
        _density_water[qp] = 1000.0;
      }
      _S_water[qp] = 1.0;
      _temp[qp] = -28.15155-(0.137458*P)+(0.3011117*H)+(3536.37/H)-(4.31919*E5*H2);
      _dTbydP_H[qp] = (-0.137458/E6);
      _dTbydH_P[qp] = (1/E3)*(0.301117-(3536.37/H2)-(8.63838*E5*H));
    }

     
//   *****************super heated steam zone****************
//   ********************************************************  
    else if (H > _enthalpy_saturated_steam[qp])
    {
      _density_steam[qp] = E3*(-b1+(b2*P)-(b3*P*H)+(b4*P4)+(b5*P*H3));
//   _ddensity_steambydP_H = ((b2)-(b3*H)+(4*b4*P3)+(b5*H3))/E3;
//   _ddensity_steambydH_P = -(b3*P)+(3*b5*P*H2);
      _S_water[qp] = 0.0;
      _temp[qp] = -374.669+(47.9921*P)-(0.633606*P2)+(7.39386*E5*H2)
        -((3.33372/E6)/(P2*H2))+(0.0357154/P3)-(1.1725*pow(10,-9)*H3*P)
        -(2.26861*pow(10,15)/pow(H,4));
      _dTbydP_H[qp] = (1/E6)*(47.9921-(1.267212*P)+((6.6744/E6)/(P3*H2))
                              -(1071462/P4)-(1.1725*pow(10,-9)*H3));
      _dTbydH_P[qp] = (1/E3)*((14.78772*E5*H)+(6.6744/(E6*P2*H3))
                              -(3.5175*pow(10,-9)*H2*P)+(9.07444*pow(10,15)/pow(H,5)));
    }
     
//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
    else 
    {
      _density_water[qp] = E3*(a1+(a2*P)-(a3*_enthalpy_saturated_water[qp])+(a4/_enthalpy_saturated_water[qp])+(a5*P*_enthalpy_saturated_water[qp])-(a6*pow(_enthalpy_saturated_water[qp],2)));
      _density_steam[qp] = E3*(-b1+(b2*P)-(b3*P*_enthalpy_saturated_steam[qp])+(b4*P4)+(b5*P*pow(_enthalpy_saturated_steam[qp],3)));
     
//     _ddensity_waterbydP_H = ((a2)+(a5*_enthalpy_saturated_water[qp]))/E3;
//     _ddensity_waterbydH_P = -(a3)-(a4/pow(_enthalpy_saturated_water[qp],2))+(a5*P)-(2*a6*_enthalpy_saturated_water[qp]);
//     _ddensity_steambydP_H = ((b2)-(b3*_enthalpy_saturated_steam[qp])+(4*b4*P3)+(b5*pow(_enthalpy_saturated_steam[qp],3)))/E3;
//     _ddensity_steambydH_P = -(b3*P)+(3*b5*P*pow(_enthalpy_saturated_steam[qp],2));
     
      a = _density_steam[qp]*(_enthalpy_saturated_steam[qp]-H);
      b = H*(_density_water[qp]-_density_steam[qp]);
      c = (_enthalpy_saturated_water[qp] * _density_water[qp])-(_enthalpy_saturated_steam[qp] * _density_steam[qp]);
      _S_water[qp] = a/(b-c);
              
      double d = 12.598833-log(10*P);
      _temp[qp] = (4667.0754/d)-273.15;
      _dTbydP_H[qp] = (1/E6)*4667.0754/(P*pow(d,2));
      _dTbydH_P[qp] = 0.0;
    }

    _S_steam[qp] = 1.0-_S_water[qp];
    _density[qp] = (_S_water[qp]*_density_water[qp])+(_S_steam[qp]*_density_steam[qp]);

//     _ddensitybydP_H[qp] = (_S_water[qp]*_ddensity_waterbydP_H)+(_S_steam[qp]*_ddensity_steambydP_H);
//     _ddensitybydH_P[qp] = (_S_water[qp]*_ddensity_waterbydH_P)+(_S_steam[qp]*_ddensity_steambydH_P);
     
    Real T = _temp[qp];
    _viscosity_steam[qp] = E7*((0.407*T)+80.4);

    double g;
    g = 247.8/(T+133.15);
    _viscosity_water[qp] = E7*241.4*pow(10,g);
    _rel_perm_water[qp] = pow(_S_water[qp],2);
    _rel_perm_steam[qp] = pow(_S_steam[qp],2);

    if ((T < 0.0) || (T >300.0))
    {
      std::cerr<<"pressure "<<_pressure[qp]<<".\n";
      std::cerr<<"enthalpy "<<_enthalpy[qp]<<".\n";
      std::cerr << "T= " << T ;
      mooseError("Temperature out of Range");
    }
      
                       
//   we used simple relative permeability function for now. we need to modify to
//   corey or vanganueten function later.     
//   compute Darcy flux and darcy params at q-points
     
    _tau_water[qp] = _permeability[qp] * _rel_perm_water[qp] * _density_water[qp] / _viscosity_water[qp];
    _darcy_flux_water[qp] =  -_permeability[qp] * _rel_perm_water[qp] / _viscosity_water[qp] * ((_grad_p[qp])+(_density_water[qp]*_gravity[qp]*_gravity_vector[qp]));
    _tau_steam[qp] = _permeability[qp] * _rel_perm_steam[qp] * _density_steam[qp] / _viscosity_steam[qp];
    _darcy_flux_steam[qp] =  -_permeability[qp] * _rel_perm_steam[qp] / _viscosity_steam[qp] * ((_grad_p[qp])+(_density_steam[qp]*_gravity[qp]*_gravity_vector[qp]));
     

     
//******************************************************************************
//  ************* this section is for computing _density_old and _temp_old **********   
//******************************************************************************
     Real H_o = _enthalpy_old[qp]/E3;
     Real P_o = _pressure_old[qp]/E6;
     Real P2_o = pow(P_o,2);  
     Real P3_o = pow(P_o,3);
     Real _enthalpy_saturated_steam_o;
     Real _enthalpy_saturated_water_o;
     Real _density_water_o;
     Real _density_steam_o;
     Real _S_water_o;
     Real _S_steam_o;

    _enthalpy_saturated_steam_o = 2822.82-(39.952/P_o)+(2.54342/P2_o)-(0.938879*P2_o);
    _enthalpy_saturated_water_o = 809.674+(94.465*P_o)-(4.502*P2_o)+(0.120265*P3_o)-(162.7/P_o)+(29.8163/P2_o)-(1.75623/P3_o);

//   compressed water zone
    if (H_o < _enthalpy_saturated_water_o)
    {
      _density_water_o = E3*(a1+(a2*P_o)-(a3*H_o)+(a4/H_o)+(a5*P_o*H_o)-(a6*pow(H_o,2)));
      _S_water_o = 1.0;
      _temp_old[qp] = -28.15155-(0.137458*P_o)+(0.3011117*H_o)+(3536.37/H_o)-(4.31919*pow(10,-5)*pow(H_o,2));
    }
//   super heated steam zone
    else if (H_o > _enthalpy_saturated_steam_o)
    {
      _density_steam_o = E3*(-b1+(b2*P_o)-(b3*P_o*H_o)+(b4*pow(P_o,4))+(b5*P_o*pow(H_o,3)));
      _S_water_o = 0.0;
      _temp_old[qp] = -374.669+(47.9921*P_o)-(0.633606*pow(P_o,2))+(7.39386*pow(10,-5)*pow(H_o,2))
        -(3.33372*pow(10,-6)/(pow(P_o,2)*pow(H_o,2)))+(0.0357154/pow(P_o,3))-(1.1725*pow(10,-9)*pow(H_o,3)*P_o)
        -(2.26861*pow(10,15)/pow(H_o,4));
    }
//   Mixed Phase Zone(Two-phase exists)
    else 
    {
      _density_water_o = E3*(a1+(a2*P_o)-(a3*_enthalpy_saturated_water_o)+(a4/_enthalpy_saturated_water_o)+(a5*P_o*_enthalpy_saturated_water_o)-(a6*pow(_enthalpy_saturated_water_o,2)));
      _density_steam_o = E3*(-b1+(b2*P_o)-(b3*P_o*_enthalpy_saturated_steam_o)+(b4*pow(P_o,4))+(b5*P_o*pow(_enthalpy_saturated_steam_o,3)));

      a = _density_steam_o*(_enthalpy_saturated_steam_o-H_o);
      b = H_o*(_density_water_o-_density_steam_o);
      c = (_enthalpy_saturated_water_o * _density_water_o)-(_enthalpy_saturated_steam_o * _density_steam_o);
      _S_water_o = a/(b-c);
      double d_o = 12.598833-log(10*P_o);
      _temp_old[qp] = (4667.0754/d_o)-273.15;
   
    }
    _S_steam_o = 1.0-_S_water_o;
    _density_old[qp] = (_S_water_o*_density_water_o)+(_S_steam_o*_density_steam_o);
     
//****************************************************************
// ******* END of _density_old and _temp_old calculation*****************
// *****************************************************************  

  }
 
}
