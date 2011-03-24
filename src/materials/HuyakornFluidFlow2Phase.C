#include "HuyakornFluidFlow2Phase.h"

template<>
InputParameters validParams<HuyakornFluidFlow2Phase>()
{
     InputParameters params = validParams<PorousMedia>();
     params.addParam<Real>("thermal_conductivity",2.4,"thermal thermal_conductivity in (W/mK)");
     params.addParam<Real>("specific_heat_rock",1000,"specific heat of rock in (J/kgK)");
     params.addCoupledVar("pressure", "TODO: add description");
     params.addCoupledVar("enthalpy", "TODO: add description");
     return params;
}

HuyakornFluidFlow2Phase::HuyakornFluidFlow2Phase(const std::string & name,
                     InputParameters parameters)
  :PorousMedia(name, parameters),
   
     _pressure(coupledValue("pressure")),
     _pressure_old(coupledValueOld("pressure")),
     _grad_p(coupledGradient("pressure")),
     _enthalpy(coupledValue("enthalpy")),
     _enthalpy_old(coupledValueOld("enthalpy")),

     _input_thermal_conductivity(getParam<Real>("thermal_conductivity")),
     _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
   
   //delcare material properties

     _km(declareProperty<Real>("thermal_conductivity")),
     _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
     _enthalpy_saturated_water(declareProperty<Real>("enthalpy_saturated_water")),
     _enthalpy_saturated_steam(declareProperty<Real>("enthalpy_saturated_steam")),
     _EOS(declareProperty<Real>("EOS")),
     _density_water(declareProperty<Real>("density_water")),
     _density_steam(declareProperty<Real>("density_steam")),
     _S_water(declareProperty<Real>("S_water")),
     _S_steam(declareProperty<Real>("S_steam")),
     _temp(declareProperty<Real>("temperature")),
     _temp_old(declareProperty<Real>("temperature_old")),
     _Heat(declareProperty<Real>("Heat")),
     _Heat_old(declareProperty<Real>("Heat_old")),
     _dT_dP(declareProperty<Real>("dT_dP")),
     _dT_dH(declareProperty<Real>("dT_dH")),
     _density(declareProperty<Real>("density")),
     _density_old(declareProperty<Real>("density_old")),
     _viscosity_water(declareProperty<Real>("viscosity_water")),
     _viscosity_steam(declareProperty<Real>("viscosity_steam")),
     _rel_perm_water(declareProperty<Real>("rel_perm_water")),
     _rel_perm_steam(declareProperty<Real>("rel_perm_steam")),
     _tau_water(declareProperty<Real>("tau_water")),
     _tau_steam(declareProperty<Real>("tau_steam")),
     _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
     _darcy_flux_steam(declareProperty<RealGradient>("darcy_flux_steam")),
     _pore_velocity_water(declareProperty<RealGradient>("pore_velocity_water")),
     _pore_velocity_steam(declareProperty<RealGradient>("pore_velocity_steam")),
     _tau(declareProperty<Real>("tau")),
     _lamda(declareProperty<Real>("lamda")),
     _beta(declareProperty<Real>("beta"))
     
{
     E3 = 1e+3;
     E6 = 1e+6;
     E7 = 1e+7;

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
     d2 = 94.4665;
     d3 = 4.50247;
     d4 = 0.120265;
     d5 = 162.7;
     d6 = 29.8163;
     d7 = 1.75623;

// coefficients related to Tw (temperature) empirical equation     
     w1 = 28.1515;
     w2 = 0.137458;
     w3 = 0.301117;
     w4 = 3536.37;
     w5 = 4.31919e-5;

// coefficients related to Ts (temperature) empirical equation     
     s1 = 374.669;
     s2 = 47.9921;
     s3 = 0.633606;
     s4 = 7.39386e-5;
     s5 = 3.3372e+6;
     s6 = 0.035714;
     s7 = 1.1725e-9;
     s8 = 2.26861e+15;
     
}

void
HuyakornFluidFlow2Phase::computeProperties()
{
  PorousMedia::computeProperties();

    
  
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {
//   we are using empirical equations to get thermodynamic data which are in Mpa and j/g......
//   we need to convert pressure from pa to Mpa and Enthalpy from J/kg to J/g

     _km[qp] = _input_thermal_conductivity;
     _specific_heat_rock[qp] = _input_specific_heat_rock;
         
     Real H  = _enthalpy[qp]/E3;
     Real P  = _pressure[qp]/E6;
     Real P2 = pow(P,2);  
     Real P3 = pow(P,3);
     Real P4 = pow(P,4);
     Real H2 = pow(H,2);
     Real H3 = pow(H,3);
     Real H4 = pow(H,4);
     Real H5 = pow(H,5);
     double a;
     double b;
     double c;
     double d;
     Real sat_T;
     Real dsat_T_dp;
     Real psi_1;
     Real psi_1_enthalpy_saturated_water;
     Real psi_2;
     Real psi_2_enthalpy_saturated_steam;
     Real enthalpy_saturated_water;
     Real enthalpy_saturated_steam;
     
     enthalpy_saturated_steam = c1-(c2/P)+(c3/P2)-(c4*P2);
     enthalpy_saturated_water = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);
     Real enthalpy_saturated_steam2 = pow(enthalpy_saturated_steam,2);
     Real enthalpy_saturated_steam3 = pow(enthalpy_saturated_steam,3);
     Real enthalpy_saturated_steam4 = pow(enthalpy_saturated_steam,4);
     Real enthalpy_saturated_water2 = pow(enthalpy_saturated_water,2);

     _enthalpy_saturated_water[qp] = E3*enthalpy_saturated_water;
     _enthalpy_saturated_steam[qp] = E3*enthalpy_saturated_steam;     
     
     
     d = 12.598833-log(10*P);
     sat_T    =  (4667.0754/d)-273.15;
     dsat_T_dp= 4667.0754/(P*pow(d,2));

//   ***********compressed water zone*************
//   ********************************************
     if (H < enthalpy_saturated_water)
       {
         _EOS[qp] = 1.0;
                            
         if (H >200.0)
           {
//comment the following line of code for FaustEx1 and
//uncomment next line of code             
             _density_water[qp] = E3*(a1+(a2*P)-(a3*H)+(a4/H)+(a5*P*H)-(a6*H2));
//             _density_water[qp] = 989.75+(4.00894e-7*_pressure[qp]);
           }
         else
           {
             _density_water[qp] = 1000.0;
           }
         _S_water[qp] = 1.0;
         psi_1 = -w1-(w2*P)+(w3*H)+(w4/H)-(w5*H2);
         psi_1_enthalpy_saturated_water = -w1-(w2*P)+(w3*enthalpy_saturated_water)+(w4/enthalpy_saturated_water)-(w5*enthalpy_saturated_water2);

//comment the following three lines of code for FaustEx1 and
//uncomment next three line of code
         _temp[qp] = psi_1 + (sat_T -psi_1_enthalpy_saturated_water);
         _dT_dP[qp] = (-w2)/E6;
         _dT_dH[qp] = (w3-(w4/H2)-(2*w5*H))/E3;

/*         _temp[qp] = -0.0208+(2.39e-4*_enthalpy[qp]);
         _dT_dP[qp] = 0.0;
         _dT_dH[qp] = 2.39e-4;
*/
       }

//   *****************super heated steam zone****************
//   ********************************************************  
     else if (H > enthalpy_saturated_steam)
       {
         _EOS[qp] = 3.0;
         _density_steam[qp] = E3*(-b1+(b2*P)-(b3*P*H)+(b4*P4)+(b5*P*H3));
         _S_water[qp] = 0.0;
         psi_2 = -s1+(s2*P)-(s3*P2)+(s4*H2)-(s5/(P2*H2))+(s6/P3)-(s7*H3*P)-(s8/H4);
         psi_2_enthalpy_saturated_steam = -s1+(s2*P)-(s3*P2)+(s4*enthalpy_saturated_steam2)-(s5/(P2*enthalpy_saturated_steam2))+(s6/P3)-(s7*enthalpy_saturated_steam3*P)-(s8/enthalpy_saturated_steam4);

         _temp[qp]  = psi_2 + (sat_T -psi_2_enthalpy_saturated_steam);
         _dT_dP[qp] = (s2-(2*s3*P)+(2*s5/(P3*H2))-(3*s6/P4)-(s7*H3))/E6;
         _dT_dH[qp] = ((2*s4*H)+(2*s5/(P2*H3))-(3*s7*P*H2)+(4*s8/H5))/E3;

       }
     
//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
     else 
       {
         _EOS[qp] = 2.0;
         _density_water[qp] = E3*(a1+(a2*P)-(a3*enthalpy_saturated_water)+(a4/enthalpy_saturated_water)+(a5*P*enthalpy_saturated_water)-(a6*enthalpy_saturated_water2));
         _density_steam[qp] = E3*(-b1+(b2*P)-(b3*P*enthalpy_saturated_steam)+(b4*P4)+(b5*P*enthalpy_saturated_steam3));
     
         a = _density_steam[qp]*(enthalpy_saturated_steam-H);
         b = H*(_density_water[qp]-_density_steam[qp]);
         c = (enthalpy_saturated_water * _density_water[qp])-(enthalpy_saturated_steam * _density_steam[qp]);
         _S_water[qp] = a/(b-c);

         _temp[qp] = sat_T;
         _dT_dP[qp] = dsat_T_dp/E6;
         _dT_dH[qp] = 0.0;
       }

     _S_steam[qp] = 1.0-_S_water[qp];
     _density[qp] = (_S_water[qp]*_density_water[qp])+(_S_steam[qp]*_density_steam[qp]);

     Real T = _temp[qp];
     _viscosity_steam[qp] = ((0.407*T)+80.4)/E7;

     double g;
     g = 247.8/(T+133.15);
     _viscosity_water[qp] = 241.4*pow(10,g)/E7;

     _rel_perm_water[qp] = pow(_S_water[qp],2);
     _rel_perm_steam[qp] = pow(_S_steam[qp],2);


     if ((T < 0.0) || (T >350.0))
       {
         std::cerr<<"pressure "<<_pressure[qp]<<".\n";
         std::cerr<<"enthalpy "<<_enthalpy[qp]<<".\n";
         std::cerr << "T= " << T ;
         mooseError("Temperature out of Range");
       }
             
                       
//   we used simple relative permeability function for now. we need to modify to
//   corey or vanganueten function later.     
//   compute darcy params, tau, lamda and beta  at q-points
     
     _tau_water[qp] = _permeability[qp] * _rel_perm_water[qp] * _density_water[qp] / _viscosity_water[qp];
     _tau_steam[qp] = _permeability[qp] * _rel_perm_steam[qp] * _density_steam[qp] / _viscosity_steam[qp];
   
     _darcy_flux_water[qp] = ( -_permeability[qp] * _rel_perm_water[qp] / _viscosity_water[qp]) * (_grad_p[qp]);
     _darcy_flux_steam[qp] = ( -_permeability[qp] * _rel_perm_steam[qp] / _viscosity_steam[qp]) * (_grad_p[qp]);

     _pore_velocity_water[qp] = _darcy_flux_water[qp]/_porosity[qp];
     _pore_velocity_steam[qp] = _darcy_flux_steam[qp]/_porosity[qp];


      
     _tau[qp] = _tau_water[qp]+_tau_steam[qp]; 

     if (_EOS[qp] == 2.0 )
       {
          _lamda[qp] = (_km[qp]*_dT_dP[qp])+(_enthalpy_saturated_water[qp]*_tau_water[qp])+(_enthalpy_saturated_steam[qp]*_tau_steam[qp]);
          _Heat[qp] = _porosity[qp]*((_density_water[qp]*_S_water[qp]*_enthalpy_saturated_water[qp])+(_density_steam[qp]*_S_steam[qp]*_enthalpy_saturated_steam[qp]))+((1-_porosity[qp])*_density_rock[qp]*_specific_heat_rock[qp]*_temp[qp]);
          
       }
     else
       {
          _lamda[qp] = (_km[qp]*_dT_dP[qp])+(_tau[qp]*_enthalpy[qp]);
          _Heat[qp] = (_porosity[qp]*_density[qp]*_enthalpy[qp])+((1-_porosity[qp])*_density_rock[qp]*_specific_heat_rock[qp]*_temp[qp]);
       }
     _beta[qp] = _km[qp]*_dT_dH[qp];

     
     
//******************************************************************************
//  ************* this section is for computing _density_old, _temp_old and Heat_old **********   
//******************************************************************************

     Real H_o = _enthalpy_old[qp]/E3;
     Real P_o = _pressure_old[qp]/E6;
     Real P2_o = pow(P_o,2);  
     Real P3_o = pow(P_o,3);
     Real P4_o = pow(P_o,4);
     Real H2_o = pow(H_o,2);
     Real H3_o = pow(H_o,3);
     Real H4_o = pow(H_o,4);
     Real _density_water_o;
     Real _density_steam_o;
     Real _S_water_o;
     Real _S_steam_o = 0;
     Real _sat_T_o;
     Real _psi_1_o;
     Real _psi_1_enthalpy_saturated_water_o;
     Real _psi_2_o;
     Real _psi_2_enthalpy_saturated_steam_o;
     Real enthalpy_saturated_water_o;
     Real enthalpy_saturated_steam_o;     
     
     enthalpy_saturated_steam_o  = c1-(c2/P_o)+(c3/P2_o)-(c4*P2_o);
     enthalpy_saturated_water_o  = d1+(d2*P_o)-(d3*P2_o)+(d4*P3_o)-(d5/P_o)+(d6/P2_o)-(d7/P3_o);
     Real enthalpy_saturated_steam2_o = pow(enthalpy_saturated_steam_o,2);
     Real enthalpy_saturated_steam3_o = pow(enthalpy_saturated_steam_o,3);
     Real enthalpy_saturated_steam4_o = pow(enthalpy_saturated_steam_o,4);
     Real enthalpy_saturated_water2_o = pow(enthalpy_saturated_water_o,2);

     d = 12.598833-log(10*P_o);
     _sat_T_o =  (4667.0754/d)-273.15;
     
     if (H_o < enthalpy_saturated_water_o)
       {
        if (H_o >200.0)
          {

//comment the following line of code for FaustEx1 and
//uncomment next line of code            
           _density_water_o = E3*(a1+(a2*P_o)-(a3*H_o)+(a4/H_o)+(a5*P_o*H_o)-(a6*H2_o));
//           _density_water_o = 989.75+(4.00894e-7*_pressure_old[qp]);

          }
        else
          {
           _density_water_o = 1000.0;
          }
       _S_water_o = 1.0;
       _psi_1_o = -w1-(w2*P_o)+(w3*H_o)+(w4/H_o)-(w5*H2_o);
       _psi_1_enthalpy_saturated_water_o = -w1-(w2*P_o)+(w3*enthalpy_saturated_water_o)+(w4/enthalpy_saturated_water_o)-(w5*enthalpy_saturated_water2_o);

//comment the following line of code for FaustEx1 and
//uncomment next line of code            
         _temp_old[qp] = _psi_1_o + (_sat_T_o -_psi_1_enthalpy_saturated_water_o);
//         _temp_old[qp] = -0.0208+(2.39e-4*_enthalpy_old[qp]);

         _Heat_old[qp] = (_porosity[qp]*_density_water_o*_enthalpy_old[qp])+((1-_porosity[qp])*_density_rock[qp]*_specific_heat_rock[qp]*_temp_old[qp]);
       
     
       }

     
//   *****************super heated steam zone****************
//   ********************************************************  
     else if (H_o > enthalpy_saturated_steam_o)
       {
       _density_steam_o = E3*(-b1+(b2*P_o)-(b3*P_o*H_o)+(b4*P4_o)+(b5*P_o*H3_o));
       _S_water_o = 0.0;
       _psi_2_o = -s1+(s2*P_o)-(s3*P2_o)+(s4*H2_o)-(s5/(P2_o*H2_o))+(s6/P3_o)-(s7*H3_o*P_o)-(s8/H4_o);
       _psi_2_enthalpy_saturated_steam_o = -s1+(s2*P_o)-(s3*P2_o)+(s4*enthalpy_saturated_steam2_o)-(s5/(P2_o*enthalpy_saturated_steam2_o))+(s6/P3_o)-(s7*enthalpy_saturated_steam3_o*P_o)-(s8/enthalpy_saturated_steam4_o);
     
       _temp_old[qp] = _psi_2_o + (_sat_T_o -_psi_2_enthalpy_saturated_steam_o);
       _Heat_old[qp] = (_porosity[qp]*_density_steam_o*_enthalpy_old[qp])+((1-_porosity[qp])*_density_rock[qp]*_specific_heat_rock[qp]*_temp_old[qp]);

       }

//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
     else 
       {
       _density_water_o = E3*(a1+(a2*P_o)-(a3*enthalpy_saturated_water_o)+(a4/enthalpy_saturated_water_o)+(a5*P_o*enthalpy_saturated_water_o)-(a6*enthalpy_saturated_water2_o));
       _density_steam_o = E3*(-b1+(b2*P_o)-(b3*P_o*enthalpy_saturated_steam_o)+(b4*P4_o)+(b5*P_o*enthalpy_saturated_steam3_o));
     
       a = _density_steam_o*(enthalpy_saturated_steam_o-H_o);
       b = H_o*(_density_water_o-_density_steam_o);
       c = (enthalpy_saturated_water_o * _density_water_o)-(enthalpy_saturated_steam_o * _density_steam_o);
       _S_water_o = a/(b-c);
       _temp_old[qp] = _sat_T_o;
       _Heat_old[qp] = _porosity[qp]*E3*((_density_water_o*_S_water_o*enthalpy_saturated_water_o)+(_density_steam_o*_S_steam_o*enthalpy_saturated_steam_o))+((1-_porosity[qp])*_density_rock[qp]*_specific_heat_rock[qp]*_temp_old[qp]);
       }
     _S_steam_o = 1.0-_S_water_o;
     _density_old[qp] = (_S_water_o*_density_water_o)+(_S_steam_o*_density_steam_o);


//****************************************************************
// ******* END of _density_old and _temp_old calculation*****************
// *****************************************************************  

 }
 
}
