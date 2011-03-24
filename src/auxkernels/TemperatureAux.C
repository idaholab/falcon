#include "TemperatureAux.h"

template<>
InputParameters validParams<TemperatureAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.addCoupledVar("pressure", "TODO: add description");
     params.addCoupledVar("enthalpy", "TODO: add description");
     return params;
}

TemperatureAux::TemperatureAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
     _pressure(coupledValue("pressure")),
     _enthalpy(coupledValue("enthalpy")),
     _temp(getMaterialProperty<Real>("temperature"))
   
{
     E3 = 1e+3;
     E6 = 1e+6;
     E7 = 1e+7;
     
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
TemperatureAux::computeValue()
{
     Real temperature;
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
     Real sat_T;
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

     
     d = 12.598833-log(10*P);
     sat_T    =  (4667.0754/d)-273.15;
     if (H < enthalpy_saturated_water)
       {
         psi_1 = -w1-(w2*P)+(w3*H)+(w4/H)-(w5*H2);
         psi_1_enthalpy_saturated_water = -w1-(w2*P)+(w3*enthalpy_saturated_water)+(w4/enthalpy_saturated_water)-(w5*enthalpy_saturated_water2);
//comment the following three lines of code for FaustEx1 and
//uncomment next three line of code
         temperature = psi_1 + (sat_T -psi_1_enthalpy_saturated_water);
//         temperature = -0.0208+(2.39e-4*_enthalpy[qp]);
       }
     else if (H > enthalpy_saturated_steam)
       {
         psi_2 = -s1+(s2*P)-(s3*P2)+(s4*H2)-(s5/(P2*H2))+(s6/P3)-(s7*H3*P)-(s8/H4);
         psi_2_enthalpy_saturated_steam = -s1+(s2*P)-(s3*P2)+(s4*enthalpy_saturated_steam2)-(s5/(P2*enthalpy_saturated_steam2))+(s6/P3)-(s7*enthalpy_saturated_steam3*P)-(s8/enthalpy_saturated_steam4);
         temperature  = psi_2 + (sat_T -psi_2_enthalpy_saturated_steam);
       }
     else 
       {
         temperature = sat_T;
       }

       return temperature;
     
//       return _temp[_qp];
     
}
