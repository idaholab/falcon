#include "CoupledTemperatureAux.h"

template<>
InputParameters validParams<CoupledTemperatureAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.addCoupledVar("pressure", "TODO: add description");
     params.addCoupledVar("enthalpy", "TODO: add description");
     return params;
}

CoupledTemperatureAux::CoupledTemperatureAux(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
     _enthalpy(coupledValue("enthalpy")),
     _pressure(coupledValue("pressure")),
     _Hw(getMaterialProperty<Real>("sat_enthalpy_w")),
     _Hs(getMaterialProperty<Real>("sat_enthalpy_s"))
{
     E3 = 1e-3;
     E5 = 1e-5;
     E6 = 1e-6;

}


Real
CoupledTemperatureAux::computeValue()
{
     Real H  = _enthalpy[_qp]* E3;
     Real P  = _pressure[_qp] * E6;
     Real P2 = pow(P,2);
     Real P3 = pow(P,3);
     Real H2 = pow(H,2);
     
     

//   compressed water zone
     if ( H <_Hw[_qp])
     {
      return -28.15155-(0.137458*P)+(0.3011117*H)+(3536.37/H)-(4.31919*E5*H2);
     }

//   super heated steam zone
     else if (H > _Hs[_qp])
     {
      return  -374.669+(47.9921*P)-(0.633606*P2)+(7.39386*E5*H2)
              -(3.33372*E6)/(P2*H2)+(0.0357154/P3)-(1.1725*pow(10,-9)*pow(H,3)*P)
              -(2.26861*pow(10,15)/pow(H,4));
     }

//   Mixed Phase Zone(Two-phase exists)
     else 
     {
       double Tw;
       double Ts;
              
       Tw =  -28.15155-(0.137458*P)+(0.3011117*_Hw[_qp])+(3536.37/_Hw[_qp])-(4.31919*E5*pow(_Hw[_qp],2));
       Ts =   -374.669+(47.9921*P)-(0.633606*P2)+(7.39386*E5*pow(_Hs[_qp],2))
         -((3.33372*E6)/(P2*pow(_Hs[_qp],2)))+(0.0357154/P3)-(1.1725*pow(10,-9)*pow(_Hs[_qp],3)*P)
         -(2.26861*pow(10,15)/pow(_Hs[_qp],4));
      
      return (Tw+Ts)/2;

//    double d = 12.598833-log(10*P);
//    return (4667.0754/d)-273.15;
     }

}
