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
     params.addParam<Real>("rel_perm_w", 1.0,"Relative permeability of water phase");
     params.addParam<Real>("rel_perm_s", 1.0,"Relative permeability of steam phase");
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
     _enthalpy( coupledValue("enthalpy")),
     _temp( coupledValue("tempAux")),

     
     _input_rho_w(parameters.get<Real>("rho_w")),
     _input_rho_s(parameters.get<Real>("rho_s")),
     _input_c_f(parameters.get<Real>("c_f")),
     _input_mu_w(parameters.get<Real>("mu_w")),
     _input_mu_s(parameters.get<Real>("mu_s")),
     _input_rel_perm_w(parameters.get<Real>("rel_perm_w")),
     _input_rel_perm_s(parameters.get<Real>("rel_perm_s")),
   
   //delcare material properties
     _rho_w(declareProperty<Real>("rho_w")),
     _rho_s(declareProperty<Real>("rho_s")),
     _rho_mix(declareProperty<Real>("rho_mix")),
     _c_f(declareProperty<Real>("c_f")),
     _mu_w(declareProperty<Real>("mu_w")),
     _mu_s(declareProperty<Real>("mu_s")),
     _rel_perm_w(declareProperty<Real>("rel_perm_w")),
     _rel_perm_s(declareProperty<Real>("rel_perm_s")),
   //  _temp(declareProperty<Real>("tempMat")),
   // _temp_old(declarePropertyOld<Real>("tempMat")),
     _sat_w(declareProperty<Real>("sat_w")),
     _sat_s(declareProperty<Real>("sat_s")),
     _dTbydP_H(declareProperty<Real>("dTbydP_H")),
     _dTbydH_P(declareProperty<Real>("dTbydH_P")),
     _sat_enthalpy_w(declareProperty<Real>("sat_enthalpy_w")),
     _sat_enthalpy_s(declareProperty<Real>("sat_enthalpy_s")),
     _darcy_params_w(declareProperty<Real>("darcy_params_w")),
     _darcy_params_s(declareProperty<Real>("darcy_params_s")),
     _darcy_flux_w(declareProperty<RealGradient>("darcy_flux_w")),
     _darcy_flux_s(declareProperty<RealGradient>("darcy_flux_s")),
     _pore_velocity_w(declareProperty<RealGradient>("pore_velocity_w")),
     _pore_velocity_s(declareProperty<RealGradient>("pore_velocity_s"))
{ }

void
FluidFlow2Phase::computeProperties()
{
  PorousMedia::computeProperties();
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {

//   fluid properties
     _rho_w[qp]               = _input_rho_w;
     _mu_w[qp]                = _input_mu_w;


//   For two phase ie water and steam
//   mean time we are using temperature as enthalpy......
//   we need to convert pressure from pa to Mpa and Enthalpy from J/kg to J/g
//
     Real H = pow(10,-3)*_enthalpy[qp];
     Real P = pow(10,-6)*_pressure[qp];

     // std:: cout << "enthalpy"<< H << ".\n";
     // std:: cout << "pressure"<< P << ".\n";
       
       
     _sat_enthalpy_s[qp] = 2822.82-(39.952/P)+(2.54342/pow(P,2))-(0.938879*pow(P,2));
     _sat_enthalpy_w[qp] = 809.674+(94.465*P)-(4.502*pow(P,2))+(0.120265*pow(P,3))-(162.7/P)+(29.8163/pow(P,2))-(1.75623/pow(P,3));

//     std:: cout << "sat_enthalpy_s"<<_sat_enthalpy_s[qp] << ".\n";
//     std:: cout << "sat_enthalpy_w"<< _sat_enthalpy_w[qp] << ".\n";
       
//   compressed water zone
     if (H < _sat_enthalpy_w[qp])
     {
     _rho_w[qp] = pow(10,3)*(0.989875+(4.00894*pow(10,-4)*P)-(4.00489*pow(10,-5)*H)+(2.66608/H)+(5.46283*pow(10,-7)*P*H)-(1.29958*pow(10,-7)*pow(H,2)));
     _sat_w[qp] = 1.0;
//     _temp[qp] = -28.15155-(0.137458*P)+(0.3011117*H)+(3536.37/H)-(4.31919*pow(10,-5)*pow(H,2));
//     _dTbydP_H[qp] = pow(10,-6)*(-0.137458);
//     _dTbydH_P[qp] = pow(10,-3)*(0.301117-(3536.37/pow(H,2))-(8.63838*pow(10,-5)*H));
     // std:: cout << "Water Zone"<< ".\n";
     
     }
//   super heated steam zone
     else if (H > _sat_enthalpy_s[qp])
     {
     _rho_s[qp] = pow(10,3)*((-2.26162*pow(10,-5))+(0.0438441*P)-(1.79088*pow(10,-5)*P*H)+(3.69276*pow(10,-8)*pow(P,4))+(5.17644*pow(10,-13)*P*pow(H,3)));
     _sat_w[qp] = 0.0;
/*     _temp[qp] = -374.669+(47.9921*P)-(0.633606*pow(P,2))+(7.39386*pow(10,-5)*pow(H,2))
                  -(3.33372*pow(10,-6)/(pow(P,2)*pow(H,2)))+(0.0357154/pow(P,3))-(1.1725*pow(10,-9)*pow(H,3)*P)
                  -(2.26861*pow(10,15)/pow(H,4));
     _dTbydP_H[qp] = pow(10,-6)*(47.9921-(1.267212*P)+(6.6744*pow(10,-6)/(pow(P,3)*pow(H,2)))
                                     -(1071462/pow(P,4))-(1.1725*pow(10,-9)*pow(H,3)));
     _dTbydH_P[qp] = pow(10,-3)*((14.78772*pow(10,-5)*H)+(6.6744*pow(10,-6)/(pow(P,2)*pow(H,3)))
                                     -(3.5175*pow(10,-9)*pow(H,2)*P)+(9.07444*pow(10,-5/pow(H,5))));
*/
//    std:: cout << "temperature"<< _temperature[qp];            
     std:: cout << "Steam Zone" << ".\n";    
     }
//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     Real Hw = _sat_enthalpy_w[qp];
     Real Hs = _sat_enthalpy_s[qp];
     _rho_w[qp] = pow(10,3)*(0.989875+(4.00894*pow(10,-4)*P)-(4.00489*pow(10,-5)*Hw)+(2.66608/Hw)+(5.46283*pow(10,-7)*P*Hw)-(1.29958*pow(10,-7)*pow(Hw,2)));
     _rho_s[qp] = pow(10,3)*((-2.26162*pow(10,-5))+(0.0438441*P)-(1.79088*pow(10,-5)*P*Hs)+(3.69276*pow(10,-8)*pow(P,4))+(5.17644*pow(10,-13)*P*pow(Hs,3)));

     double a = _rho_s[qp]*(Hs-H);
     double b = H*(_rho_w[qp]-_rho_s[qp]);
     double c = (Hw * _rho_w[qp])-(Hs * _rho_s[qp]);
     _sat_w[qp] = a/(b-c);
         
//   Real Tw = -28.15155-(0.137458*P)+(0.3011117*Hw)+(3536.37/Hw)-(4.31919*pow(10,-5)*pow(Hw,2));
//   Real Ts = -374.669+(47.9921*P)-(0.633606*pow(P,2))+(7.39386*pow(10,-5)*pow(Hs,2))
//                  -(3.33372*pow(10,-6)/(pow(P,2)*pow(Hs,2)))+(0.0357154/pow(P,3))-(1.1725*pow(10,-9)*pow(Hs,3)*P)
//                  -(2.26861*pow(10,15)/pow(Hs,4));
//   _temp[qp] = (Tw+Ts)/2.0;
//
/*     double d = 12.598833-log(10*P);
     _temp[qp] = (4667.0754/d)-273.15;
     _dTbydP_H[qp] = pow(10,-6)*4667.0754/(P*pow(d,2));
     _dTbydH_P[qp] = 0.0;
*/
     // std:: cout << "two-phase Zone"<< ".\n";

     }
     _mu_s[qp] = pow(10,-7)*((0.407*_temp[qp])+80.4);
     double e = 247.8/(_temp[qp]+133.15);
     _mu_w[qp] = pow(10,-7)*241.4*pow(10,e);
     _sat_s[qp] = 1.0-_sat_w[qp];
     _rho_mix[qp] = (_sat_w[qp]*_rho_w[qp])+(_sat_s[qp]*_rho_s[qp]);
     
     _rel_perm_w[qp] = pow(_sat_w[qp],2);
     _rel_perm_s[qp] = pow(_sat_s[qp],2);

//     std::cout << "rel_perm_w" << _rel_perm_w[qp]<<".\n";
//     std::cout << "rel_perm_s" << _rel_perm_s[qp]<<".\n";
//     std::cout << "mu_s" << _mu_s[qp]<<".\n";
//     std::cout << "rel_perm_s" << _rel_perm_s[qp]<<".\n";
//          std::cout << "rel_perm_w" << _rel_perm_w[qp]<<".\n";
//     std::cout << "rel_perm_s" << _rel_perm_s[qp]<<".\n";
//          std::cout << "rel_perm_w" << _rel_perm_w[qp]<<".\n";
//     std::cout << "rel_perm_s" << _rel_perm_s[qp]<<".\n";
     
     
//   we used simple relative permeability function for now. we need to modify to
//   corey or vanganueten function later.     
    
//   compute Darcy flux and pore water velicity on q-points
     
     _darcy_params_w[qp] = _permeability[qp] * _rel_perm_w[qp] * _rho_w[qp] / _mu_w[qp];
     _darcy_flux_w[qp] =  -_permeability[qp] * _rel_perm_w[qp] / _mu_w[qp] * ((_grad_p[qp])+(_rho_w[qp]*_gravity[qp]*_gravity_vector[qp]));
     _pore_velocity_w[qp] = _darcy_flux_w[qp] / _porosity[qp];
     _darcy_params_s[qp] = _permeability[qp] * _rel_perm_s[qp] * _rho_s[qp] / _mu_s[qp];
     _darcy_flux_s[qp] =  -_permeability[qp] * _rel_perm_s[qp] / _mu_s[qp] * ((_grad_p[qp])+(_rho_s[qp]*_gravity[qp]*_gravity_vector[qp]));
     _pore_velocity_s[qp] = _darcy_flux_s[qp] / _porosity[qp];

     // std::cout << "darcy_params_w" << _darcy_params_w[qp]<<".\n";
     //std::cout << "darcy_params_s" << _darcy_params_s[qp]<<".\n";
     //std::cout << "darcy_flux_w" << _darcy_flux_w[qp]<<".\n";
     //std::cout << "darcy_flux_s" << _darcy_flux_s[qp]<<".\n";
  }
}
