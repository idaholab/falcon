#include "WaterSatAux.h"

template<>
InputParameters validParams<WaterSatAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("enthalpy", "TODO: add description");
  return params;
}

WaterSatAux::WaterSatAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
    _pressure(coupledValue("pressure")),
    _enthalpy(coupledValue("enthalpy")),
    _sat_w(getMaterialProperty<Real>("sat_w"))

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

}

Real
WaterSatAux::computeValue()
{
     Real saturation;
     Real H  = _enthalpy[_qp]/E3;
     Real P  = _pressure[_qp]/E6;
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
     Real Hw;
     Real Hs;
     Real _rho_w;
     Real _rho_s;
     
     Hs = c1-(c2/P)+(c3/P2)-(c4*P2);
     Hw = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);
     Real Hs2 = pow(Hs,2);
     Real Hs3 = pow(Hs,3);
     Real Hs4 = pow(Hs,4);
     Real Hw2 = pow(Hw,2);
     
     if (H < Hw)
       {
         saturation = 1.0;
       }
     else if (H > Hs)
       {
         saturation = 0.0;
       }
     else 
       {
         _rho_w = E3*(a1+(a2*P)-(a3*Hw)+(a4/Hw)+(a5*P*Hw)-(a6*Hw2));
         _rho_s = E3*(-b1+(b2*P)-(b3*P*Hs)+(b4*P4)+(b5*P*Hs3));
     
         a = _rho_s*(Hs-H);
         b = H*(_rho_w-_rho_s);
         c = (Hw * _rho_w)-(Hs * _rho_s);
         saturation = a/(b-c);
       }
 
 return _sat_w[_qp];
 return saturation;
 
}
