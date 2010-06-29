
#include "CoupledDarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<CoupledDarcyImplicitEuler>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("enthalpy", "TODO: add description");
  return params;
}

CoupledDarcyImplicitEuler::CoupledDarcyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
     _porosity(getRealMaterialProperty("porosity")),
     _enthalpy(coupledVal("enthalpy")),
     _enthalpy_old(coupledValOld("enthalpy"))
   
{}

Real
CoupledDarcyImplicitEuler::computeQpResidual()
{
     Real H = _enthalpy[_qp] * pow(10,-3);
     Real H_old = _enthalpy_old[_qp] * pow(10,-3);
     Real P = _u[_qp] * pow(10,-6);
     Real P_old = _u_old[_qp] * pow(10,-6);
     Real  _sat_w;
     Real  _sat_w_old;
     Real  _rho;
     Real  _rho_old;
     Real  _rho_s;
     Real  _rho_s_old;
     Real  _rho_w;
     Real  _rho_w_old;
     Real  Hw;
     Real  Hs;
     Real  Hw_old;
     Real  Hs_old;
     
     Hs = 2822.82-(39.952/P)+(2.54342/pow(P,2))-(0.938879*pow(P,2));
     Hw = 809.674+(94.465*P)-(4.502*pow(P,2))+(0.120265*pow(P,3))-(162.7/P)+(29.8163/pow(P,2))-(1.75623/pow(P,3));

     Hs_old = 2822.82-(39.952/P_old)+(2.54342/pow(P_old,2))-(0.938879*pow(P_old,2));
     Hw_old = 809.674+(94.465*P_old)-(4.502*pow(P_old,2))+(0.120265*pow(P_old,3))-(162.7/P_old)+(29.8163/pow(P_old,2))-(1.75623/pow(P_old,3));
 
//   compressed water zone
     if ( H < Hw)
     {
     _rho = pow(10,3)*(0.989875+(4.00894*pow(10,-4)*P)-(4.00489*pow(10,-5)*H)+(2.66608/H)+(5.46283*pow(10,-7)*P*H)-(1.29958*pow(10,-7)*pow(H,2)));
     _sat_w = 1.0;
     }

//   super heated steam zone
     else if (H > Hs)
     {
     _rho = pow(10,3)*((-2.26162*pow(10,-5))+(0.0438441*P)-(1.79088*pow(10,-5)*P*H)+(3.69276*pow(10,-8)*pow(P,4))+(5.17644*pow(10,-13)*P*pow(H,3)));
     _sat_w = 0.0;
     }

//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     _rho_w = pow(10,3)*(0.989875+(4.00894*pow(10,-4)*P)-(4.00489*pow(10,-5)*Hw)+(2.66608/Hw)+(5.46283*pow(10,-7)*P*Hw)-(1.29958*pow(10,-7)*pow(Hw,2)));
     _rho_s = pow(10,3)*((-2.26162*pow(10,-5))+(0.0438441*P)-(1.79088*pow(10,-5)*P*Hs)+(3.69276*pow(10,-8)*pow(P,4))+(5.17644*pow(10,-13)*P*pow(Hs,3)));
     double a = _rho_s*(Hs-H);
     double b = H*(_rho_w -_rho_s);
     double c = (Hw * _rho_w)-(Hs * _rho_s);
     _sat_w = a/(b-c);
     _rho = (_sat_w * _rho_w)+((1-_sat_w)*_rho_s);
     }

//   compressed water zone
     if ( H_old < Hw_old)
     {
     _rho_old = pow(10,3)*(0.989875+(4.00894*pow(10,-4)*P_old)-(4.00489*pow(10,-5)*H_old)+(2.66608/H_old)+(5.46283*pow(10,-7)*P_old*H_old)-(1.29958*pow(10,-7)*pow(H_old,2)));
     _sat_w_old = 1.0;
     }

//   super heated steam zone
     else if (H_old > Hs_old)
     {
     _rho_old = pow(10,3)*((-2.26162*pow(10,-5))+(0.0438441*P_old)-(1.79088*pow(10,-5)*P_old*H_old)+(3.69276*pow(10,-8)*pow(P_old,4))+(5.17644*pow(10,-13)*P*pow(H_old,3)));
     _sat_w_old = 0.0;
     }

//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     _rho_w_old = pow(10,3)*(0.989875+(4.00894*pow(10,-4)*P_old)-(4.00489*pow(10,-5)*Hw_old)+(2.66608/Hw_old)+(5.46283*pow(10,-7)*P_old*Hw_old)-(1.29958*pow(10,-7)*pow(Hw_old,2)));
     _rho_s_old = pow(10,3)*((-2.26162*pow(10,-5))+(0.0438441*P_old)-(1.79088*pow(10,-5)*P_old*Hs_old)+(3.69276*pow(10,-8)*pow(P_old,4))+(5.17644*pow(10,-13)*P*pow(Hs_old,3)));
     double a = _rho_s_old*(Hs_old-H_old);
     double b = H_old*(_rho_w_old -_rho_s_old);
     double c = (Hw_old * _rho_w_old)-(Hs_old * _rho_s_old);
     _sat_w_old = a/(b-c);
     _rho_old = (_sat_w_old * _rho_w_old)+((1-_sat_w_old)*_rho_s_old);
     }     

     //   Real residue1;
     // residue1 = _porosity[_qp]*_test[_i][_qp]*(_rho -_rho_old)/_dt;

     // std::cout << "rho" << _rho << " .\n";
     // std::cout << "rho_old" << _rho_old << " .\n";
     

    
     return _porosity[_qp]*_test[_i][_qp]*(_rho -_rho_old)/_dt;
}

Real
CoupledDarcyImplicitEuler::computeQpJacobian()
{
     Real H = _enthalpy[_qp] * pow(10,-3);
     Real P = _u[_qp] * pow(10,-6);
     Real  _sat_w;
     Real  _rho;
     Real  _rho_s;
     Real  _rho_w;
     Real  Hw;
     Real  Hs;
     Real  dHw;
     Real  dHs;
     
     Hs = 2822.82-(39.952/P)+(2.54342/pow(P,2))-(0.938879*pow(P,2));
     Hw = 809.674+(94.465*P)-(4.502*pow(P,2))+(0.120265*pow(P,3))-(162.7/P)+(29.8163/pow(P,2))-(1.75623/pow(P,3));
     dHs = pow(10,-6)*((-39.952*pow(P,-2)*_phi[_j][_qp])+(-2*2.54342*pow(P,-3)*_phi[_j][_qp])-(0.938879*P*2*_phi[_j][_qp]));
     dHw = pow(10,-6)*((94.465*_phi[_j][_qp])-(4.502*P*2*_phi[_j][_qp])+(0.120265*3*pow(P,2)*_phi[_j][_qp])-(-162.7*pow(P,-2)*_phi[_j][_qp])+(-2*29.8163*pow(P,3)*_phi[_j][_qp])-(-3*1.75623*pow(P,-4)*_phi[_j][_qp]));

     
//   compressed water zone
     if ( H < Hw)
     {
     _rho = pow(10,3)*((4.00894*pow(10,-10)*_phi[_j][_qp])+(5.46283*pow(10,-13)*_phi[_j][_qp]*H));
     _sat_w = 1.0;
     }

//   super heated steam zone
     else if (H > Hs)
     {
     _rho = pow(10,3)*((0.0438441*pow(10,-6)*_phi[_j][_qp])-(1.79088*pow(10,-11)*_phi[_j][_qp]*H)+(3.69276*pow(10,-14)*4*pow(P,3)*_phi[_j][_qp])+(5.17644*pow(10,-19)*_phi[_j][_qp]*pow(H,3)));
     _sat_w = 0.0;
     }

//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     _rho_w = pow(10,3)*((4.00894*pow(10,-10)*_phi[_j][_qp])-(4.00489*pow(10,-5)*dHw)+(2.66608/dHw)+(5.46283*pow(10,-7)*P*dHw)+(5.46283*pow(10,-13)*_phi[_j][_qp]*Hw)-(1.29958*pow(10,-7)*Hw*2*dHw));
     _rho_s = pow(10,3)*((0.0438441*pow(10,-6)*_phi[_j][_qp])-(1.79088*pow(10,-5)*P*dHs)-(1.79088*pow(10,-11)*_phi[_j][_qp]*Hs)+(3.69276*pow(10,-8)*4*pow(P,3)*_phi[_j][_qp])+(5.17644*pow(10,-19)*_phi[_j][_qp]*pow(Hs,3))+(5.17644*pow(10,-13)*P*3*pow(Hs,2)*dHs));
     double a = _rho_s*(Hs-H);
     double b = H*(_rho_w -_rho_s);
     double c = (Hw * _rho_w)-(Hs * _rho_s);
     _sat_w = a/(b-c);
     _rho = (_sat_w * _rho_w)+((1-_sat_w)*_rho_s);
     }
     return _porosity[_qp]*_test[_i][_qp]*(_rho)/_dt;

}
