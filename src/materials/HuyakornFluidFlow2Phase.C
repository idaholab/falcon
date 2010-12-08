#include "HuyakornFluidFlow2Phase.h"

template<>
InputParameters validParams<HuyakornFluidFlow2Phase>()
{
     InputParameters params = validParams<PorousMedia>();
     params.addParam<Real>("thermal_conductivity",2.4,"thermal thermal_conductivity in (W/mK)");
     params.addParam<Real>("cp_r",1000,"specific heat of rock in (J/kgK)");
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
     _input_cp_r(getParam<Real>("cp_r")),
   
   //delcare material properties

     _km(declareProperty<Real>("thermal_conductivity")),
     _cp_r(declareProperty<Real>("cp_r")),
     _Hw(declareProperty<Real>("sat_enthalpy_w")),
     _Hs(declareProperty<Real>("sat_enthalpy_s")),
     _EOS(declareProperty<Real>("EOS")),
     _rho_w(declareProperty<Real>("rho_w")),
     _rho_s(declareProperty<Real>("rho_s")),
     _sat_w(declareProperty<Real>("sat_w")),
     _sat_s(declareProperty<Real>("sat_s")),
     _temp(declareProperty<Real>("temperature")),
     _temp_old(declareProperty<Real>("temperature_old")),
     _Heat(declareProperty<Real>("Heat")),
     _Heat_old(declareProperty<Real>("Heat_old")),
     _dT_dP(declareProperty<Real>("dT_dP")),
     _dT_dH(declareProperty<Real>("dT_dH")),
     _rho(declareProperty<Real>("rho")),
     _rho_old(declareProperty<Real>("rho_old")),
     _mu_w(declareProperty<Real>("mu_w")),
     _mu_s(declareProperty<Real>("mu_s")),
     _rel_perm_w(declareProperty<Real>("rel_perm_w")),
     _rel_perm_s(declareProperty<Real>("rel_perm_s")),
     _darcy_params_w(declareProperty<Real>("darcy_params_w")),
     _darcy_params_s(declareProperty<Real>("darcy_params_s")),
     _darcy_flux_w(declareProperty<RealGradient>("darcy_flux_w")),
     _darcy_flux_s(declareProperty<RealGradient>("darcy_flux_s")),
     _pore_velocity_w(declareProperty<RealGradient>("pore_velocity_w")),
     _pore_velocity_s(declareProperty<RealGradient>("pore_velocity_s")),
     _tau(declareProperty<Real>("tau")),
     _lamda(declareProperty<Real>("lamda")),
     _beta(declareProperty<Real>("beta"))
     
{
     E3 = 1e+3;
     E6 = 1e+6;
     E7 = 1e+7;

// coefficients related to rho_w empirical equation     
     a1 = 0.989875;
     a2 = 4.00894e-4;
     a3 = 4.00489e-5;
     a4 = 2.66608;
     a5 = 5.46283e-7;
     a6 = 1.29958e-7;

// coefficients related to rho_s empirical equation
     b1 = 2.26162e-5;
     b2 = 0.0438441;
     b3 = 1.79088e-5;
     b4 = 3.69276e-8;
     b5 = 5.17644e-13;

// coefficients related to Hs empirical equation     
     c1 = 2822.82;
     c2 = 39.952;
     c3 = 2.54342;
     c4 = 0.938879;

// coefficients related to Hw empirical equation     
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
     _cp_r[qp] = _input_cp_r;
         
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
     Real psi_1_Hw;
     Real psi_2;
     Real psi_2_Hs;
     Real Hw;
     Real Hs;
     
     Hs = c1-(c2/P)+(c3/P2)-(c4*P2);
     Hw = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);
     Real Hs2 = pow(Hs,2);
     Real Hs3 = pow(Hs,3);
     Real Hs4 = pow(Hs,4);
     Real Hw2 = pow(Hw,2);

     _Hw[qp] = E3*Hw;
     _Hs[qp] = E3*Hs;     
     
     
     d = 12.598833-log(10*P);
     sat_T    =  (4667.0754/d)-273.15;
     dsat_T_dp= 4667.0754/(P*pow(d,2));

//   ***********compressed water zone*************
//   ********************************************
     if (H < Hw)
       {
         _EOS[qp] = 1.0;
                            
         if (H >200.0)
           {
//comment the following line of code for FaustEx1 and
//uncomment next line of code             
             _rho_w[qp] = E3*(a1+(a2*P)-(a3*H)+(a4/H)+(a5*P*H)-(a6*H2));
//             _rho_w[qp] = 989.75+(4.00894e-7*_pressure[qp]);
           }
         else
           {
             _rho_w[qp] = 1000.0;
           }
         _sat_w[qp] = 1.0;
         psi_1 = -w1-(w2*P)+(w3*H)+(w4/H)-(w5*H2);
         psi_1_Hw = -w1-(w2*P)+(w3*Hw)+(w4/Hw)-(w5*Hw2);

//comment the following three lines of code for FaustEx1 and
//uncomment next three line of code
         _temp[qp] = psi_1 + (sat_T -psi_1_Hw);
         _dT_dP[qp] = (-w2)/E6;
         _dT_dH[qp] = (w3-(w4/H2)-(2*w5*H))/E3;

/*         _temp[qp] = -0.0208+(2.39e-4*_enthalpy[qp]);
         _dT_dP[qp] = 0.0;
         _dT_dH[qp] = 2.39e-4;
*/
       }

//   *****************super heated steam zone****************
//   ********************************************************  
     else if (H > Hs)
       {
         _EOS[qp] = 3.0;
         _rho_s[qp] = E3*(-b1+(b2*P)-(b3*P*H)+(b4*P4)+(b5*P*H3));
         _sat_w[qp] = 0.0;
         psi_2 = -s1+(s2*P)-(s3*P2)+(s4*H2)-(s5/(P2*H2))+(s6/P3)-(s7*H3*P)-(s8/H4);
         psi_2_Hs = -s1+(s2*P)-(s3*P2)+(s4*Hs2)-(s5/(P2*Hs2))+(s6/P3)-(s7*Hs3*P)-(s8/Hs4);

         _temp[qp]  = psi_2 + (sat_T -psi_2_Hs);
         _dT_dP[qp] = (s2-(2*s3*P)+(2*s5/(P3*H2))-(3*s6/P4)-(s7*H3))/E6;
         _dT_dH[qp] = ((2*s4*H)+(2*s5/(P2*H3))-(3*s7*P*H2)+(4*s8/H5))/E3;

       }
     
//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
     else 
       {
         _EOS[qp] = 2.0;
         _rho_w[qp] = E3*(a1+(a2*P)-(a3*Hw)+(a4/Hw)+(a5*P*Hw)-(a6*Hw2));
         _rho_s[qp] = E3*(-b1+(b2*P)-(b3*P*Hs)+(b4*P4)+(b5*P*Hs3));
     
         a = _rho_s[qp]*(Hs-H);
         b = H*(_rho_w[qp]-_rho_s[qp]);
         c = (Hw * _rho_w[qp])-(Hs * _rho_s[qp]);
         _sat_w[qp] = a/(b-c);

         _temp[qp] = sat_T;
         _dT_dP[qp] = dsat_T_dp/E6;
         _dT_dH[qp] = 0.0;
       }

     _sat_s[qp] = 1.0-_sat_w[qp];
     _rho[qp] = (_sat_w[qp]*_rho_w[qp])+(_sat_s[qp]*_rho_s[qp]);

     Real T = _temp[qp];
     _mu_s[qp] = ((0.407*T)+80.4)/E7;

     double g;
     g = 247.8/(T+133.15);
     _mu_w[qp] = 241.4*pow(10,g)/E7;

     _rel_perm_w[qp] = pow(_sat_w[qp],2);
     _rel_perm_s[qp] = pow(_sat_s[qp],2);


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
     
     _darcy_params_w[qp] = _permeability[qp] * _rel_perm_w[qp] * _rho_w[qp] / _mu_w[qp];
     _darcy_params_s[qp] = _permeability[qp] * _rel_perm_s[qp] * _rho_s[qp] / _mu_s[qp];
   
     _darcy_flux_w[qp] = ( -_permeability[qp] * _rel_perm_w[qp] / _mu_w[qp]) * (_grad_p[qp]);
     _darcy_flux_s[qp] = ( -_permeability[qp] * _rel_perm_s[qp] / _mu_s[qp]) * (_grad_p[qp]);

     _pore_velocity_w[qp] = _darcy_flux_w[qp]/_porosity[qp];
     _pore_velocity_s[qp] = _darcy_flux_s[qp]/_porosity[qp];


      
     _tau[qp] = _darcy_params_w[qp]+_darcy_params_s[qp]; 

     if (_EOS[qp] == 2.0 )
       {
          _lamda[qp] = (_km[qp]*_dT_dP[qp])+(_Hw[qp]*_darcy_params_w[qp])+(_Hs[qp]*_darcy_params_s[qp]);
          _Heat[qp] = _porosity[qp]*((_rho_w[qp]*_sat_w[qp]*_Hw[qp])+(_rho_s[qp]*_sat_s[qp]*_Hs[qp]))+((1-_porosity[qp])*_rho_r[qp]*_cp_r[qp]*_temp[qp]);
          
       }
     else
       {
          _lamda[qp] = (_km[qp]*_dT_dP[qp])+(_tau[qp]*_enthalpy[qp]);
          _Heat[qp] = (_porosity[qp]*_rho[qp]*_enthalpy[qp])+((1-_porosity[qp])*_rho_r[qp]*_cp_r[qp]*_temp[qp]);
       }
     _beta[qp] = _km[qp]*_dT_dH[qp];

     
     
//******************************************************************************
//  ************* this section is for computing _rho_old, _temp_old and Heat_old **********   
//******************************************************************************

     Real H_o = _enthalpy_old[qp]/E3;
     Real P_o = _pressure_old[qp]/E6;
     Real P2_o = pow(P_o,2);  
     Real P3_o = pow(P_o,3);
     Real P4_o = pow(P_o,4);
     Real H2_o = pow(H_o,2);
     Real H3_o = pow(H_o,3);
     Real H4_o = pow(H_o,4);
     Real _rho_w_o;
     Real _rho_s_o;
     Real _sat_w_o;
     Real _sat_s_o = 0;
     Real _sat_T_o;
     Real _psi_1_o;
     Real _psi_1_Hw_o;
     Real _psi_2_o;
     Real _psi_2_Hs_o;
     Real Hw_o;
     Real Hs_o;     
     
     Hs_o  = c1-(c2/P_o)+(c3/P2_o)-(c4*P2_o);
     Hw_o  = d1+(d2*P_o)-(d3*P2_o)+(d4*P3_o)-(d5/P_o)+(d6/P2_o)-(d7/P3_o);
     Real Hs2_o = pow(Hs_o,2);
     Real Hs3_o = pow(Hs_o,3);
     Real Hs4_o = pow(Hs_o,4);
     Real Hw2_o = pow(Hw_o,2);

     d = 12.598833-log(10*P_o);
     _sat_T_o =  (4667.0754/d)-273.15;
     
     if (H_o < Hw_o)
       {
        if (H_o >200.0)
          {

//comment the following line of code for FaustEx1 and
//uncomment next line of code            
           _rho_w_o = E3*(a1+(a2*P_o)-(a3*H_o)+(a4/H_o)+(a5*P_o*H_o)-(a6*H2_o));
//           _rho_w_o = 989.75+(4.00894e-7*_pressure_old[qp]);

          }
        else
          {
           _rho_w_o = 1000.0;
          }
       _sat_w_o = 1.0;
       _psi_1_o = -w1-(w2*P_o)+(w3*H_o)+(w4/H_o)-(w5*H2_o);
       _psi_1_Hw_o = -w1-(w2*P_o)+(w3*Hw_o)+(w4/Hw_o)-(w5*Hw2_o);

//comment the following line of code for FaustEx1 and
//uncomment next line of code            
         _temp_old[qp] = _psi_1_o + (_sat_T_o -_psi_1_Hw_o);
//         _temp_old[qp] = -0.0208+(2.39e-4*_enthalpy_old[qp]);

         _Heat_old[qp] = (_porosity[qp]*_rho_w_o*_enthalpy_old[qp])+((1-_porosity[qp])*_rho_r[qp]*_cp_r[qp]*_temp_old[qp]);
       
     
       }

     
//   *****************super heated steam zone****************
//   ********************************************************  
     else if (H_o > Hs_o)
       {
       _rho_s_o = E3*(-b1+(b2*P_o)-(b3*P_o*H_o)+(b4*P4_o)+(b5*P_o*H3_o));
       _sat_w_o = 0.0;
       _psi_2_o = -s1+(s2*P_o)-(s3*P2_o)+(s4*H2_o)-(s5/(P2_o*H2_o))+(s6/P3_o)-(s7*H3_o*P_o)-(s8/H4_o);
       _psi_2_Hs_o = -s1+(s2*P_o)-(s3*P2_o)+(s4*Hs2_o)-(s5/(P2_o*Hs2_o))+(s6/P3_o)-(s7*Hs3_o*P_o)-(s8/Hs4_o);
     
       _temp_old[qp] = _psi_2_o + (_sat_T_o -_psi_2_Hs_o);
       _Heat_old[qp] = (_porosity[qp]*_rho_s_o*_enthalpy_old[qp])+((1-_porosity[qp])*_rho_r[qp]*_cp_r[qp]*_temp_old[qp]);

       }

//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
     else 
       {
       _rho_w_o = E3*(a1+(a2*P_o)-(a3*Hw_o)+(a4/Hw_o)+(a5*P_o*Hw_o)-(a6*Hw2_o));
       _rho_s_o = E3*(-b1+(b2*P_o)-(b3*P_o*Hs_o)+(b4*P4_o)+(b5*P_o*Hs3_o));
     
       a = _rho_s_o*(Hs_o-H_o);
       b = H_o*(_rho_w_o-_rho_s_o);
       c = (Hw_o * _rho_w_o)-(Hs_o * _rho_s_o);
       _sat_w_o = a/(b-c);
       _temp_old[qp] = _sat_T_o;
       _Heat_old[qp] = _porosity[qp]*E3*((_rho_w_o*_sat_w_o*Hw_o)+(_rho_s_o*_sat_s_o*Hs_o))+((1-_porosity[qp])*_rho_r[qp]*_cp_r[qp]*_temp_old[qp]);
       }
     _sat_s_o = 1.0-_sat_w_o;
     _rho_old[qp] = (_sat_w_o*_rho_w_o)+(_sat_s_o*_rho_s_o);


//****************************************************************
// ******* END of _rho_old and _temp_old calculation*****************
// *****************************************************************  

 }
 
}
