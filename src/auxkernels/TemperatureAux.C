#include "TemperatureAux.h"

template<>
InputParameters validParams<TemperatureAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.addCoupledVar("pressure", "TODO: add description");
     params.addCoupledVar("enthalpy", "TODO: add description");
     return params;
}

TemperatureAux::TemperatureAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
     _pressure(coupledValue("pressure")),
     _enthalpy(coupledValue("enthalpy")),
     _temp(getMaterialProperty<Real>("temperature"))
   
{
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

     
     d = 12.598833-log(10*P);
     sat_T    =  (4667.0754/d)-273.15;
     if (H < Hw)
       {
         psi_1 = -w1-(w2*P)+(w3*H)+(w4/H)-(w5*H2);
         psi_1_Hw = -w1-(w2*P)+(w3*Hw)+(w4/Hw)-(w5*Hw2);
//comment the following three lines of code for FaustEx1 and
//uncomment next three line of code
         temperature = psi_1 + (sat_T -psi_1_Hw);
//         temperature = -0.0208+(2.39e-4*_enthalpy[qp]);
       }
     else if (H > Hs)
       {
         psi_2 = -s1+(s2*P)-(s3*P2)+(s4*H2)-(s5/(P2*H2))+(s6/P3)-(s7*H3*P)-(s8/H4);
         psi_2_Hs = -s1+(s2*P)-(s3*P2)+(s4*Hs2)-(s5/(P2*Hs2))+(s6/P3)-(s7*Hs3*P)-(s8/Hs4);
         temperature  = psi_2 + (sat_T -psi_2_Hs);
       }
     else 
       {
         temperature = sat_T;
       }

       return temperature;
     
//       return _temp[_qp];
     
}
