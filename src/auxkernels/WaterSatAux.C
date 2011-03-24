#include "WaterSatAux.h"

template<>
InputParameters validParams<WaterSatAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("enthalpy", "TODO: add description");
  return params;
}

WaterSatAux::WaterSatAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
    _pressure(coupledValue("pressure")),
    _enthalpy(coupledValue("enthalpy")),
    _S_water(getMaterialProperty<Real>("S_water"))

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
     Real enthalpy_saturated_water;
     Real enthalpy_saturated_steam;
     Real _density_water;
     Real _density_steam;
     
     enthalpy_saturated_steam = c1-(c2/P)+(c3/P2)-(c4*P2);
     enthalpy_saturated_water = d1+(d2*P)-(d3*P2)+(d4*P3)-(d5/P)+(d6/P2)-(d7/P3);
     Real enthalpy_saturated_steam2 = pow(enthalpy_saturated_steam,2);
     Real enthalpy_saturated_steam3 = pow(enthalpy_saturated_steam,3);
     Real enthalpy_saturated_steam4 = pow(enthalpy_saturated_steam,4);
     Real enthalpy_saturated_water2 = pow(enthalpy_saturated_water,2);
     
     if (H < enthalpy_saturated_water)
       {
         saturation = 1.0;
       }
     else if (H > enthalpy_saturated_steam)
       {
         saturation = 0.0;
       }
     else 
       {
         _density_water = E3*(a1+(a2*P)-(a3*enthalpy_saturated_water)+(a4/enthalpy_saturated_water)+(a5*P*enthalpy_saturated_water)-(a6*enthalpy_saturated_water2));
         _density_steam = E3*(-b1+(b2*P)-(b3*P*enthalpy_saturated_steam)+(b4*P4)+(b5*P*enthalpy_saturated_steam3));
     
         a = _density_steam*(enthalpy_saturated_steam-H);
         b = H*(_density_water-_density_steam);
         c = (enthalpy_saturated_water * _density_water)-(enthalpy_saturated_steam * _density_steam);
         saturation = a/(b-c);
       }
 
// return _S_water[_qp];
 return saturation;
 
}
