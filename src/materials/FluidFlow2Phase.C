#include "FluidFlow2Phase.h"

template<>
InputParameters validParams<FluidFlow2Phase>()
{
     InputParameters params = validParams<PorousMedia>(); //inherit porous medium basic properties
     params.addParam<Real>("rho_w",1000.0,"fluid density in Kg/m^3");
     params.addParam<Real>("rho_s",1.0,"steam density in Kg/m^3");
     params.addParam<Real>("c_f", 4.6e-10,"fluid compressibility in 1/Pa");
     params.addParam<Real>("mu_w",0.001,"fluid dynamic viscosity in Pa.s");
     params.addParam<Real>("mu_s",0.001,"steam dynamic viscosity in Pa.s");
     params.addCoupledVar("pressure", "TODO: add description");
     params.addCoupledVar("enthalpy", "TODO: add description");
     params.addCoupledVar("tempAux", "TODO: add description");
     return params;
}

FluidFlow2Phase::FluidFlow2Phase(std::string name,
                     MooseSystem & moose_system,
                     InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),

     _grad_p  ( coupledGradient("pressure")),
     _pressure( coupledValue("pressure")),
     _pressure_old( coupledValueOld("pressure")),
     _enthalpy( coupledValue("enthalpy")),
     _enthalpy_old( coupledValueOld("enthalpy")),
     _temp( coupledValue("tempAux")),

     
     _input_rho_w(parameters.get<Real>("rho_w")),
     _input_rho_s(parameters.get<Real>("rho_s")),
     _input_c_f(parameters.get<Real>("c_f")),
     _input_mu_w(parameters.get<Real>("mu_w")),
     _input_mu_s(parameters.get<Real>("mu_s")),

   
   //delcare material properties

     _rho_w(declareProperty<Real>("rho_w")),
     _rho_s(declareProperty<Real>("rho_s")),
     _rho(declareProperty<Real>("rho")),
     _rho_old(declareProperty<Real>("rho_old")),
     _c_f(declareProperty<Real>("c_f")),
     _mu_w(declareProperty<Real>("mu_w")),
     _mu_s(declareProperty<Real>("mu_s")),
     _rel_perm_w(declareProperty<Real>("rel_perm_w")),
     _rel_perm_s(declareProperty<Real>("rel_perm_s")),
   //  _temp(declareProperty<Real>("tempMat")),
   // _temp_old(declarePropertyOld<Real>("tempMat")),
     _sat_w(declareProperty<Real>("sat_w")),
     _sat_s(declareProperty<Real>("sat_s")),
     _Hw(declareProperty<Real>("sat_enthalpy_w")),
     _Hs(declareProperty<Real>("sat_enthalpy_s")),
     _GHw(declareProperty<RealGradient>("grad_sat_enthalpy_w")),
     _GHs(declareProperty<RealGradient>("grad_sat_enthalpy_s")),
     _darcy_params_w(declareProperty<Real>("darcy_params_w")),
     _darcy_params_s(declareProperty<Real>("darcy_params_s")),
     _darcy_flux_w(declareProperty<RealGradient>("darcy_flux_w")),
     _darcy_flux_s(declareProperty<RealGradient>("darcy_flux_s")),
     _pore_velocity_w(declareProperty<RealGradient>("pore_velocity_w")),
     _pore_velocity_s(declareProperty<RealGradient>("pore_velocity_s"))

{
     E3 = pow(10,3);
     E6 = pow(10,6);
     E5 = pow(10,-5);
     E7 = pow(10,-7);

// coefficients related to rho_w empirical equation     
     a1 = 0.989875;
     a2 = 4.00894*pow(10,-4);
     a3 = 4.00489*E5;
     a4 = 2.66608;
     a5 = 5.46283*E7;
     a6 = 1.29958*E7;

// coefficients related to rho_s empirical equation
     b1 = -2.26162*E5;
     b2 = 0.0438441;
     b3 = 1.79088*E5;
     b4 = 3.69276*pow(10,-8);
     b5 = 5.17644*pow(10,-13);

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

  try 
  {
    
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {

//   fluid properties
     _rho_w[qp]               = _input_rho_w;
     _mu_w[qp]                = _input_mu_w;


//   For two phase ie water and steam
//   we are using empirical equations to get thermodynamic data which are in Mpa and j/g......
//   we need to convert pressure from pa to Mpa and Enthalpy from J/kg to J/g
//

     Real H = _enthalpy[qp]/E3;
     Real P = _pressure[qp]/E6;
     Real P2 = pow(P,2);  
     Real P3 = pow(P,3);
     
     _Hs[qp]  = c1-(c2/P)+(c3/P2)-(c4*P2);
     _GHs[qp] = _grad_p[qp]/E3*((c2/P2)-(c3*2/P3)-(c4*2*P));  
     _Hw[qp]  = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);
     _GHw[qp] = _grad_p[qp]/E3*((d2)-(d3*2*P)+(d4*3*P2)+(d5/P2)-(d6*2/P3)+(d7*3*pow(P,-4)));
     
//   compressed water zone
     if (H < _Hw[qp])
     {
     _rho_w[qp] = E3*(a1+(a2*P)-(a3*H)+(a4/H)+(a5*P*H)-(a6*pow(H,2)));
     _sat_w[qp] = 1.0;
     }
//   super heated steam zone
     else if (H > _Hs[qp])
     {
     _rho_s[qp] = E3*(b1+(b2*P)-(b3*P*H)+(b4*pow(P,4))+(b5*P*pow(H,3)));
     _sat_w[qp] = 0.0;
     }
//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     _rho_w[qp] = E3*(a1+(a2*P)-(a3*_Hw[qp])+(a4/_Hw[qp])+(a5*P*_Hw[qp])-(a6*pow(_Hw[qp],2)));
     _rho_s[qp] = E3*(b1+(b2*P)-(b3*P*_Hs[qp])+(b4*pow(P,4))+(b5*P*pow(_Hs[qp],3)));

     double a = _rho_s[qp]*(_Hs[qp]-H);
     double b = H*(_rho_w[qp]-_rho_s[qp]);
     double c = (_Hw[qp] * _rho_w[qp])-(_Hs[qp] * _rho_s[qp]);
     _sat_w[qp] = a/(b-c);
     }
     _sat_s[qp] = 1.0-_sat_w[qp];
     _rho[qp] = (_sat_w[qp]*_rho_w[qp])+(_sat_s[qp]*_rho_s[qp]);

//   this section is for computing _rho_old     
     Real H_o = _enthalpy_old[qp]/E3;
     Real P_o = _pressure_old[qp]/E6;
     Real P2_o = pow(P,2);  
     Real P3_o = pow(P,3);
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
     }
//   super heated steam zone
     else if (H_o > _Hs_o)
     {
     _rho_s_o = E3*(b1+(b2*P_o)-(b3*P_o*H_o)+(b4*pow(P_o,4))+(b5*P_o*pow(H_o,3)));
     _sat_w_o = 0.0;
     }
//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     _rho_w_o = E3*(a1+(a2*P_o)-(a3*_Hw_o)+(a4/_Hw_o)+(a5*P_o*_Hw_o)-(a6*pow(_Hw_o,2)));
     _rho_s_o = E3*(b1+(b2*P_o)-(b3*P_o*_Hs_o)+(b4*pow(P_o,4))+(b5*P_o*pow(_Hs_o,3)));

     double d = _rho_s_o*(_Hs_o-H_o);
     double e = H_o*(_rho_w_o-_rho_s_o);
     double f = (_Hw_o * _rho_w_o)-(_Hs_o * _rho_s_o);
     _sat_w_o = d/(e-f);
     }
           
     _sat_s_o = 1.0-_sat_w_o;
     _rho_old[qp] = (_sat_w_o*_rho_w_o)+(_sat_s_o*_rho_s_o);

     _mu_s[qp] = E7*((0.407*_temp[qp])+80.4);
     double g = 247.8/(_temp[qp]+133.15);
     _mu_w[qp] = E7*241.4*pow(10,g);

     
     _rel_perm_w[qp] = pow(_sat_w[qp],2);
     _rel_perm_s[qp] = pow(_sat_s[qp],2);

     
     
//   we used simple relative permeability function for now. we need to modify to
//   corey or vanganueten function later.     
    
//   compute Darcy flux and pore water velicity on q-points
     
     _darcy_params_w[qp] = _permeability[qp] * _rel_perm_w[qp] * _rho_w[qp] / _mu_w[qp];
     
     mooseAssert(std::fabs(_mu_w[qp]) > 1e-5, "_mu_w is zero: ");
     mooseAssert(!std::isnan(_darcy_params_w[qp]), "_darcy_params_w is NaN");
                 
     _darcy_flux_w[qp] =  -_permeability[qp] * _rel_perm_w[qp] / _mu_w[qp] * ((_grad_p[qp])+(_rho_w[qp]*_gravity[qp]*_gravity_vector[qp]));

     
     
     _pore_velocity_w[qp] = _darcy_flux_w[qp] / _porosity[qp];
     _darcy_params_s[qp] = _permeability[qp] * _rel_perm_s[qp] * _rho_s[qp] / _mu_s[qp];
     _darcy_flux_s[qp] =  -_permeability[qp] * _rel_perm_s[qp] / _mu_s[qp] * ((_grad_p[qp])+(_rho_s[qp]*_gravity[qp]*_gravity_vector[qp]));
     _pore_velocity_s[qp] = _darcy_flux_s[qp] / _porosity[qp];

     
     
  }

  }
  catch(...)
  {
    std::cout << "exception caught in FluidFlow2Phase.C";
  }
}
