#include "FluidFlow2Phase.h"

template<>
InputParameters validParams<FluidFlow2Phase>()
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

FluidFlow2Phase::FluidFlow2Phase(std::string name,
                                 MooseSystem & moose_system,
                                 InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),

   _grad_p  (coupledGradient("pressure")),
   _pressure(coupledValue("pressure")),
   _pressure_old(coupledValueOld("pressure")),
   _enthalpy(coupledValue("enthalpy")),
   _enthalpy_old(coupledValueOld("enthalpy")),
//   _temp(coupledValue("tempAux")),
//   _temp_old(coupledValueOld("tempAux")),

   
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
  E3 = 1e3;
  E6 = 1e6;
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
    _rho_w[qp]               = _input_rho_w;
    _mu_w[qp]                = _input_mu_w;
    _rho_s[qp]               = _input_rho_s;
    _mu_s[qp]                = _input_mu_s;

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
    Real _drho_wbydP_H = 1;
    Real _drho_wbydH_P = 1;
    Real _drho_sbydP_H = 1;
    Real _drho_sbydH_P = 1;

     
    _Hs[qp]  = c1-(c2/P)+(c3/P2)-(c4*P2);
    _Hw[qp]  = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);

//   _GHs[qp] = _grad_p[qp]/E3*((c2/P2)-(c3*2/P3)-(c4*2*P));  
//   _GHw[qp] = _grad_p[qp]/E3*((d2)-(d3*2*P)+(d4*3*P2)+(d5/P2)-(d6*2/P3)+(d7*3/P4));
     

     
//   ***********compressed water zone*************
//   ********************************************
    if (H < _Hw[qp])
    {
      if (H >200.0)
      {
        _rho_w[qp] = E3*(a1+(a2*P)-(a3*H)+(a4/H)+(a5*P*H)-(a6*H2));
         
//   _drho_wbydP_H = ((a2)+(a5*H))/E3;
//   _drho_wbydH_P = -(a3)-(a4/H2)+(a5*P)-(2*a6*H);
      }
      else
      {
        _rho_w[qp] = 1000.0;
      }
      _sat_w[qp] = 1.0;
      _temp[qp] = -28.15155-(0.137458*P)+(0.3011117*H)+(3536.37/H)-(4.31919*E5*H2);
      _dTbydP_H[qp] = (-0.137458/E6);
      _dTbydH_P[qp] = (1/E3)*(0.301117-(3536.37/H2)-(8.63838*E5*H));
    }

     
//   *****************super heated steam zone****************
//   ********************************************************  
    else if (H > _Hs[qp])
    {
      _rho_s[qp] = E3*(b1+(b2*P)-(b3*P*H)+(b4*P4)+(b5*P*H3));
//   _drho_sbydP_H = ((b2)-(b3*H)+(4*b4*P3)+(b5*H3))/E3;
//   _drho_sbydH_P = -(b3*P)+(3*b5*P*H2);
      _sat_w[qp] = 0.0;
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
      _rho_w[qp] = E3*(a1+(a2*P)-(a3*_Hw[qp])+(a4/_Hw[qp])+(a5*P*_Hw[qp])-(a6*pow(_Hw[qp],2)));
      _rho_s[qp] = E3*(b1+(b2*P)-(b3*P*_Hs[qp])+(b4*P4)+(b5*P*pow(_Hs[qp],3)));
     
//     _drho_wbydP_H = ((a2)+(a5*_Hw[qp]))/E3;
//     _drho_wbydH_P = -(a3)-(a4/pow(_Hw[qp],2))+(a5*P)-(2*a6*_Hw[qp]);
//     _drho_sbydP_H = ((b2)-(b3*_Hs[qp])+(4*b4*P3)+(b5*pow(_Hs[qp],3)))/E3;
//     _drho_sbydH_P = -(b3*P)+(3*b5*P*pow(_Hs[qp],2));
     
      a = _rho_s[qp]*(_Hs[qp]-H);
      b = H*(_rho_w[qp]-_rho_s[qp]);
      c = (_Hw[qp] * _rho_w[qp])-(_Hs[qp] * _rho_s[qp]);
      _sat_w[qp] = a/(b-c);
              
      double d = 12.598833-log(10*P);
      _temp[qp] = (4667.0754/d)-273.15;
      _dTbydP_H[qp] = (1/E6)*4667.0754/(P*pow(d,2));
      _dTbydH_P[qp] = 0.0;
    }

    _sat_s[qp] = 1.0-_sat_w[qp];
    _rho[qp] = (_sat_w[qp]*_rho_w[qp])+(_sat_s[qp]*_rho_s[qp]);

//     _drhobydP_H[qp] = (_sat_w[qp]*_drho_wbydP_H)+(_sat_s[qp]*_drho_sbydP_H);
//     _drhobydH_P[qp] = (_sat_w[qp]*_drho_wbydH_P)+(_sat_s[qp]*_drho_sbydH_P);
     
    Real T = _temp[qp];
    _mu_s[qp] = E7*((0.407*T)+80.4);

    double g;
    g = 247.8/(T+133.15);
    _mu_w[qp] = E7*241.4*pow(10,g);
    _rel_perm_w[qp] = pow(_sat_w[qp],2);
    _rel_perm_s[qp] = pow(_sat_s[qp],2);

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
     
    _darcy_params_w[qp] = _permeability[qp] * _rel_perm_w[qp] * _rho_w[qp] / _mu_w[qp];
    _darcy_flux_w[qp] =  -_permeability[qp] * _rel_perm_w[qp] / _mu_w[qp] * ((_grad_p[qp])+(_rho_w[qp]*_gravity[qp]*_gravity_vector[qp]));
    _darcy_params_s[qp] = _permeability[qp] * _rel_perm_s[qp] * _rho_s[qp] / _mu_s[qp];
    _darcy_flux_s[qp] =  -_permeability[qp] * _rel_perm_s[qp] / _mu_s[qp] * ((_grad_p[qp])+(_rho_s[qp]*_gravity[qp]*_gravity_vector[qp]));
     

     
//******************************************************************************
//  ************* this section is for computing _rho_old and _temp_old **********   
//******************************************************************************
     Real H_o = _enthalpy_old[qp]/E3;
     Real P_o = _pressure_old[qp]/E6;
     Real P2_o = pow(P_o,2);  
     Real P3_o = pow(P_o,3);
     Real _Hs_o;
     Real _Hw_o;
     Real _rho_w_o;
     Real _rho_s_o;
     Real _sat_w_o;
     Real _sat_s_o;

    _Hs_o = 2822.82-(39.952/P_o)+(2.54342/P2_o)-(0.938879*P2_o);
    _Hw_o = 809.674+(94.465*P_o)-(4.502*P2_o)+(0.120265*P3_o)-(162.7/P_o)+(29.8163/P2_o)-(1.75623/P3_o);

//   compressed water zone
    if (H_o < _Hw_o)
    {
      _rho_w_o = E3*(a1+(a2*P_o)-(a3*H_o)+(a4/H_o)+(a5*P_o*H_o)-(a6*pow(H_o,2)));
      _sat_w_o = 1.0;
      _temp_old[qp] = -28.15155-(0.137458*P_o)+(0.3011117*H_o)+(3536.37/H_o)-(4.31919*pow(10,-5)*pow(H_o,2));
    }
//   super heated steam zone
    else if (H_o > _Hs_o)
    {
      _rho_s_o = E3*(b1+(b2*P_o)-(b3*P_o*H_o)+(b4*pow(P_o,4))+(b5*P_o*pow(H_o,3)));
      _sat_w_o = 0.0;
      _temp_old[qp] = -374.669+(47.9921*P_o)-(0.633606*pow(P_o,2))+(7.39386*pow(10,-5)*pow(H_o,2))
        -(3.33372*pow(10,-6)/(pow(P_o,2)*pow(H_o,2)))+(0.0357154/pow(P_o,3))-(1.1725*pow(10,-9)*pow(H_o,3)*P_o)
        -(2.26861*pow(10,15)/pow(H_o,4));
    }
//   Mixed Phase Zone(Two-phase exists)
    else 
    {
      _rho_w_o = E3*(a1+(a2*P_o)-(a3*_Hw_o)+(a4/_Hw_o)+(a5*P_o*_Hw_o)-(a6*pow(_Hw_o,2)));
      _rho_s_o = E3*(b1+(b2*P_o)-(b3*P_o*_Hs_o)+(b4*pow(P_o,4))+(b5*P_o*pow(_Hs_o,3)));

      a = _rho_s_o*(_Hs_o-H_o);
      b = H_o*(_rho_w_o-_rho_s_o);
      c = (_Hw_o * _rho_w_o)-(_Hs_o * _rho_s_o);
      _sat_w_o = a/(b-c);
      double d_o = 12.598833-log(10*P_o);
      _temp_old[qp] = (4667.0754/d_o)-273.15;
   
    }
    _sat_s_o = 1.0-_sat_w_o;
    _rho_old[qp] = (_sat_w_o*_rho_w_o)+(_sat_s_o*_rho_s_o);
     
//****************************************************************
// ******* END of _rho_old and _temp_old calculation*****************
// *****************************************************************  

  }
 
}
