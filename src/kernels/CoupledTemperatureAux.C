#include "CoupledTemperatureAux.h"

template<>
InputParameters validParams<CoupledTemperatureAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("enthalpy", "TODO: add description");
  return params;
}

CoupledTemperatureAux::CoupledTemperatureAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
   _enthalpy(coupledValue("enthalpy")),
   _pressure(coupledValue("pressure"))
   
{}


Real
CoupledTemperatureAux::computeValue()
{
     Real H =_enthalpy[_qp] * pow(10,-3);
     Real P =_pressure[_qp] * pow(10,-6);
     Real  Hw;
     Real  Hs;

     Hs = 2822.82-(39.952/P)+(2.54342/pow(P,2))-(0.938879*pow(P,2));
     Hw = 809.674+(94.465*P)-(4.502*pow(P,2))+(0.120265*pow(P,3))-(162.7/P)+(29.8163/pow(P,2))-(1.75623/pow(P,3));
     
 
//   compressed water zone
     if ( H < Hw)
     {
              
       return -28.15155-(0.137458*P)+(0.3011117*H)+(3536.37/H)-(4.31919*pow(10,-5)*pow(H,2));
     }

//   super heated steam zone
     else if (H > Hs)
     {
     return  -374.669+(47.9921*P)-(0.633606*pow(P,2))+(7.39386*pow(10,-5)*pow(H,2))
                  -(3.33372*pow(10,-6)/(pow(P,2)*pow(H,2)))+(0.0357154/pow(P,3))-(1.1725*pow(10,-9)*pow(H,3)*P)
                  -(2.26861*pow(10,15)/pow(H,4));
     }

//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     double d = 12.598833-log(10*P);
     return (4667.0754/d)-273.15;
     }

}
