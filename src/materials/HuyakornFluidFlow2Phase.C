#include "HuyakornFluidFlow2Phase.h"

template<>
InputParameters validParams<HuyakornFluidFlow2Phase>()
{
     InputParameters params = validParams<PorousMedia>(); //inherit porous medium basic properties
     params.addParam<Real>("rho_w",1000.0,"fluid density in Kg/m^3");
     params.addParam<Real>("rho_s",0.01,"steam density in Kg/m^3");
     params.addParam<Real>("c_f", 4.6e-10,"fluid compressibility in 1/Pa");
     params.addParam<Real>("mu_w",0.001,"fluid dynamic viscosity in Pa.s");
     params.addParam<Real>("mu_s",0.00001,"steam dynamic viscosity in Pa.s");
     params.addCoupledVar("pressure", "TODO: add description");
     params.addCoupledVar("enthalpy", "TODO: add description");
     params.addCoupledVar("tempAux", "TODO: add description");
     
     return params;
}

HuyakornFluidFlow2Phase::HuyakornFluidFlow2Phase(std::string name,
                     MooseSystem & moose_system,
                     InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),

     _grad_p  (coupledGradient("pressure")),
     _pressure(coupledValue("pressure")),
     _pressure_old(coupledValueOld("pressure")),
//     _enthalpy(coupledValue("enthalpy")),
//     _enthalpy_old(coupledValueOld("enthalpy")),

   
     _input_rho_w(parameters.get<Real>("rho_w")),
     _input_rho_s(parameters.get<Real>("rho_s")),
     _input_c_f(parameters.get<Real>("c_f")),
     _input_mu_w(parameters.get<Real>("mu_w")),
     _input_mu_s(parameters.get<Real>("mu_s")),

   
   //delcare material properties

     _rho_w(declareProperty<Real>("rho_w")),
     _rho_s(declareProperty<Real>("rho_s")),
     _c_f(declareProperty<Real>("c_f")),
     _mu_w(declareProperty<Real>("mu_w")),
     _mu_s(declareProperty<Real>("mu_s")),
     _rel_perm_w(declareProperty<Real>("rel_perm_w")),
     _rel_perm_s(declareProperty<Real>("rel_perm_s")),

     _temp(declareProperty<Real>("temperature")),
     _temp_old(declareProperty<Real>("temperature_old")),
     _dTbydP_H(declareProperty<Real>("dTbydP_H")),
     _dTbydH_P(declareProperty<Real>("dTbydH_P")),
     
     _rho(declareProperty<Real>("rho")),
     _rho_old(declareProperty<Real>("rho_old")),
     _drhobydP_H(declareProperty<Real>("drhobydP_H")),
     _drhobydH_P(declareProperty<Real>("drhobydH_P")),
   
     _sat_w(declareProperty<Real>("sat_w")),
     _sat_s(declareProperty<Real>("sat_s")),
     _Hw(declareProperty<Real>("sat_enthalpy_w")),
     _Hs(declareProperty<Real>("sat_enthalpy_s")),
     _GHw(declareProperty<RealGradient>("grad_sat_enthalpy_w")),
     _GHs(declareProperty<RealGradient>("grad_sat_enthalpy_s")),
     _darcy_params_w(declareProperty<Real>("darcy_params_w")),
     _darcy_params_s(declareProperty<Real>("darcy_params_s")),
     _darcy_flux_w(declareProperty<RealGradient>("darcy_flux_w")),
     _darcy_flux_s(declareProperty<RealGradient>("darcy_flux_s"))

     
{
     E3 = 1e+3;
     E6 = 1e+6;
     E5 = 1e-5;
     E7 = 1e-7;

// coefficients related to rho_w empirical equation     
     a1 = 0.989875;
     a2 = 4.00894e-4;
     a3 = 4.00489e-5;
     a4 = 2.66608;
     a5 = 5.46283e-7;
     a6 = 1.29958e-7;

// coefficients related to rho_s empirical equation
     b1 = -2.26162e-5;
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

  
    
//   fluid properties
     _rho_w[qp]               = _input_rho_w;
     _mu_w[qp]                = _input_mu_w;
     _rho_s[qp]               = _input_rho_s;
     _mu_s[qp]                = _input_mu_s;

//   For two phase ie water and steam
//   we are using empirical equations to get thermodynamic data which are in Mpa and j/g......
//   we need to convert pressure from pa to Mpa and Enthalpy from J/kg to J/g

     Real _enthalpy = 300000;
     
     Real H  = _enthalpy/E3;
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
     Real sat_T;
     Real dsat_T_dp;
     Real psi_1;
     Real psi_1_Hw;
     Real psi_2;
     Real psi_2_Hs;
   
       if ((P < 0.001) || (P >20.0))
       {
         std::cerr<<"pressure "<<_pressure[qp]<<".\n";
         std::cerr<<"enthalpy "<<_enthalpy<<".\n";
         std::cout<<"_rho[_qp] "<<_rho[_qp]<<".\n";
         std::cout<<"_rho_old[_qp] "<<_rho_old[_qp]<<".\n";
         std::cout<<"porosity "<<_porosity[_qp]<<".\n";
         std::cout<<"darcy_params_w "<<_darcy_params_w[qp]<<".\n";
         std::cout<<"darcy_params_s "<<_darcy_params_s[qp]<<".\n"; 

         mooseError("pressure out of Range");
       }
   
     
//    std::cout<<"pressure "<<_pressure[qp]<<".\n";
//    std::cout<<"enthalpy "<<_enthalpy<<".\n";
     
     _Hs[qp]  = c1-(c2/P)+(c3/P2)-(c4*P2);
     _Hw[qp]  = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);

     Real Hw  = _Hw[qp];
     Real Hs  = _Hs[qp];     
     Real Hs2 = pow(_Hs[qp],2);
     Real Hs3 = pow(_Hs[qp],3);
     Real Hs4 = pow(_Hs[qp],4);
     Real Hw2 = pow(_Hw[qp],2);
     
     double d = 12.598833-log(10*P);
     sat_T    =  (4667.0754/d)-273.15;
     dsat_T_dp= (1/E6)*4667.0754/(P*pow(d,2));
     
//    std::cout<<"sat_T "<<sat_T<<".\n";
     
     
     
//   ***********compressed water zone*************
//   ********************************************
     if (H < Hw)
       {
         if (H >200.0)
           {
             _rho_w[qp] = E3*(a1+(a2*P)-(a3*H)+(a4/H)+(a5*P*H)-(a6*H2));
         
           }
         else
           {
             _rho_w[qp] = 1000.0;
           }
         _sat_w[qp] = 1.0;
         psi_1 = -w1-(w2*P)+(w3*H)+(w4/H)-(w5*H2);
         psi_1_Hw = -w1-(w2*P)+(w3*Hw)+(w4/Hw)-(w5*Hw2);
         _temp[qp] = psi_1 + (sat_T -psi_1_Hw);

//         std::cout<<"psi_1 "<<psi_1<<".\n";
//         std::cout<<"psi_1_Hw "<<psi_1_Hw<<".\n";
         
         _dTbydP_H[qp] = dsat_T_dp;
         _dTbydH_P[qp] = w3-(w4/H2)-(2*w5*H);
     
       }

     
//   *****************super heated steam zone****************
//   ********************************************************  
     else if (H > Hs)
       {
         _rho_s[qp] = E3*(b1+(b2*P)-(b3*P*H)+(b4*P4)+(b5*P*H3));
         _sat_w[qp] = 0.0;
         psi_2 = -s1+(s2*P)-(s3*P2)+(s4*H2)-(s5/(P2*H2))+(s6/P3)-(s7*H3*P)-(s8/H4);
         psi_2_Hs = -s1+(s2*P)-(s3*P2)+(s4*Hs2)-(s5/(P2*Hs2))+(s6/P3)-(s7*Hs3*P)-(s8/Hs4);

         Real dpsi_2_dp = s2-(2*s3*P)+(2*s5/(P3*H2))-(3*s6/P4)-(s7*H3);
         Real dpsi_2_Hs_dp = s2-(2*s3*P)+(2*s5/(P3*Hs2))-(3*s6/P4)-(s7*Hs3);
           
         _temp[qp]  = psi_2 + (sat_T -psi_2_Hs);
         _dTbydP_H[qp] = dpsi_2_dp + (dsat_T_dp -dpsi_2_Hs_dp) ;
         _dTbydH_P[qp] = (2*s4*H)-(2*s5*P2*H3)-(3*s7*P*H2)+(4*s8*H5);

       }
     
//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
     else 
       {
         _rho_w[qp] = E3*(a1+(a2*P)-(a3*Hw)+(a4/Hw)+(a5*P*Hw)-(a6*Hw2));
         _rho_s[qp] = E3*(b1+(b2*P)-(b3*P*Hs)+(b4*P4)+(b5*P*Hs3));
     
         a = _rho_s[qp]*(Hs-H);
         b = H*(_rho_w[qp]-_rho_s[qp]);
         c = (Hw * _rho_w[qp])-(Hs * _rho_s[qp]);
         _sat_w[qp] = a/(b-c);
              
         _temp[qp] = sat_T;
         _dTbydP_H[qp] = dsat_T_dp;
         _dTbydH_P[qp] = 0.0;
       }

     _sat_s[qp] = 1.0-_sat_w[qp];
     _rho[qp] = (_sat_w[qp]*_rho_w[qp])+(_sat_s[qp]*_rho_s[qp]);

     Real T = _temp[qp];
     
     //   std::cout<<"_temperature "<<T<<".\n";
     _mu_s[qp] = E7*((0.407*T)+80.4);

     double g;
     g = 247.8/(T+133.15);
     _mu_w[qp] = E7*241.4*pow(10,g);


//     std::cout<<"_mu_w[qp] "<<_mu_w[qp]<<".\n";
//     std::cout<<"_mu_s[qp] "<<_mu_s[qp]<<".\n";
 
     _rel_perm_w[qp] = pow(_sat_w[qp],2);
     _rel_perm_s[qp] = pow(_sat_s[qp],2);

//     _rel_perm_w[qp] = 1.0;
//     _rel_perm_s[qp] = 0.0;

//     std::cout<<"_rel_perm_w[qp] "<<_rel_perm_w[qp]<<".\n";
//     std::cout<<"_rel_perm_s[qp] "<<_rel_perm_s[qp]<<".\n";

     if ((T < 0.0) || (T >350.0))
       {
         std::cerr<<"pressure "<<_pressure[qp]<<".\n";
         std::cerr<<"enthalpy "<<_enthalpy<<".\n";
         std::cerr << "T= " << T ;
         mooseError("Temperature out of Range");
       }
             
                       
//   we used simple relative permeability function for now. we need to modify to
//   corey or vanganueten function later.     
//   compute Darcy flux and darcy params at q-points
     
     _darcy_params_w[qp] = _permeability[qp] * _rel_perm_w[qp] * _rho_w[qp] / _mu_w[qp];
     _darcy_flux_w[qp] =  -_permeability[qp] * _rel_perm_w[qp] / _mu_w[qp] * ((_grad_p[qp])+(_rho_w[qp]*_gravity[qp]*_gravity_vector[qp]));
     _darcy_params_s[qp] = _permeability[qp] * _rel_perm_s[qp] * _rho_s[qp] / _mu_s[qp];
     _darcy_flux_s[qp] =  -_permeability[qp] * _rel_perm_s[qp] / _mu_s[qp] * ((_grad_p[qp])+(_rho_s[qp]*_gravity[qp]*_gravity_vector[qp]));


//     std::cout<<"darcy_params_w "<<_darcy_params_w[qp]<<".\n";
//     std::cout<<"darcy_params_s "<<_darcy_params_s[qp]<<".\n";
//     std::cout<<"row_w "<<_rho_w[qp]<<".\n";
         

     
//******************************************************************************
//  ************* this section is for computing _rho_old and _temp_old **********   
//******************************************************************************

     Real _enthalpy_old = 300000;
     

     Real H_o = _enthalpy_old/E3;
     Real P_o = _pressure_old[qp]/E6;
     Real P2_o = pow(P_o,2);  
     Real P3_o = pow(P_o,3);
     Real P4_o = pow(P_o,4);
     Real H2_o = pow(H_o,2);
     Real H3_o = pow(H_o,3);
     Real _rho_w_o;
     Real _rho_s_o;
     Real _sat_w_o;
     Real _sat_s_o;

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
     Real Hw2_o = pow(Hw_o,2);

     double d_o = 12.598833-log(10*P_o);
     _sat_T_o =  (4667.0754/d_o)-273.15;
     
     if (H_o < Hw_o)
       {
        if (H_o >200.0)
          {
           _rho_w_o = E3*(a1+(a2*P_o)-(a3*H_o)+(a4/H_o)+(a5*P*H_o)-(a6*H2_o));
          }
        else
          {
           _rho_w_o = 1000.0;
          }
       _sat_w_o = 1.0;
       _psi_1_o = -w1-(w2*P_o)+(w3*H_o)+(w4/H_o)-(w5*H2_o);
       _psi_1_Hw_o = -w1-(w2*P_o)+(w3*Hw_o)+(w4/Hw_o)-(w5*Hw2_o);
       _temp_old[qp] = _psi_1_o + (_sat_T_o -_psi_1_Hw_o);
     
       }

     
//   *****************super heated steam zone****************
//   ********************************************************  
     else if (H_o > Hs_o)
       {
       _rho_s_o = E3*(b1+(b2*P_o)-(b3*P_o*H_o)+(b4*P4_o)+(b5*P_o*H3_o));
       _sat_w_o = 0.0;
       _psi_2_o = -s1+(s2*P_o)-(s3*P2_o)+(s4*H2_o)-(s5/(P2*H2_o))+(s6/P3_o)-(s7*H3_o*P_o)-(s8/pow(H_o,4));
       _psi_2_Hs_o = -s1+(s2*P_o)-(s3*P2_o)+(s4*Hs2_o)-(s5/(P2_o*Hs2_o))+(s6/P3_o)-(s7*Hs3_o*P_o)
                 -(s8/pow(Hs_o,4));
     
       _temp_old[qp] = _psi_2_o + (_sat_T_o -_psi_2_Hs_o);

       }

//   ****************Mixed Phase Zone(Two-phase exists)**********
//   ***********************************************************
     else 
       {
       _rho_w_o = E3*(a1+(a2*P_o)-(a3*Hw_o)+(a4/Hw_o)+(a5*P_o*Hw_o)-(a6*Hw2_o));
       _rho_s_o = E3*(b1+(b2*P_o)-(b3*P_o*Hs_o)+(b4*P4_o)+(b5*P_o*Hs3_o));
     
       a = _rho_s_o*(Hs_o-H_o);
       b = H_o*(_rho_w_o-_rho_s_o);
       c = (Hw_o * _rho_w_o)-(Hs_o * _rho_s_o);
       _sat_w_o = a/(b-c);
              
       _temp_old[qp] = _sat_T_o;
     
       }

     _sat_s_o = 1.0-_sat_w_o;
     _rho_old[qp] = (_sat_w_o*_rho_w_o)+(_sat_s_o*_rho_s_o);

/*
     std::cout<<"sat_w_o "<<_sat_w_o<<".\n";
     std::cout<<"sat_s_o "<<_sat_s_o<<".\n";
     std::cout<<"_rho_w_o "<<_rho_w_o<<".\n";
     std::cout<<"_rho_s_o "<<_rho_s_o<<".\n";
     std::cout<<"pressure_o "<<_pressure_old[qp]<<".\n";
     std::cout<<"enthalpy_o "<<_enthalpy_old<<".\n";
     std::cout<<"_row_old "<<_rho_old[qp]<<".\n";
*/         
//****************************************************************
// ******* END of _rho_old and _temp_old calculation*****************
// *****************************************************************  

 }
 
}
