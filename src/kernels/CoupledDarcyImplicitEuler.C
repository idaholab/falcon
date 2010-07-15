
#include "CoupledDarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<CoupledDarcyImplicitEuler>()
{
     InputParameters params = validParams<ImplicitEuler>();
     params.addCoupledVar("enthalpy", "TODO: add description");
     params.addCoupledVar("rhoAux", "TODO: add description");
     return params;
}

CoupledDarcyImplicitEuler::CoupledDarcyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),

//     _rho(coupledValue("rhoAux")),
//     _rho_old(coupledValueOld("rhoAux")),

   _rho(getMaterialProperty<Real>("rho")),
   _rho_old(getMaterialProperty<Real>("rho_old")),

   _enthalpy(coupledValue("enthalpy")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _sat_w(getMaterialProperty<Real>("sat_w")),
   _sat_s(getMaterialProperty<Real>("sat_s")),
   _Hw(getMaterialProperty<Real>("sat_enthalpy_w")),
   _Hs(getMaterialProperty<Real>("sat_enthalpy_s"))
   
        
{
     E3 = pow(10,3);
     E6 = pow(10,6);
     
     a2 = 4.00894*pow(10,-4);
     a5 = 5.46283*pow(10,-7);
     
     b2 = 0.0438441;
     b3 = 1.79088*pow(10,-5);
     b4 = 3.69276*pow(10,-8);
     b5 = 5.17644*pow(10,-13);
}

Real
CoupledDarcyImplicitEuler::computeQpResidual()
{ 

     return _porosity[_qp]*_test[_i][_qp]*(_rho[_qp] -_rho_old[_qp])/_dt;
}

Real
CoupledDarcyImplicitEuler::computeQpJacobian()
{    
  Real H = _enthalpy[_qp]/E3;
  Real P = _u[_qp]/E6;
  Real P2 = pow(P,2);  
  Real P3 = pow(P,3);
  Real P4 = pow(P,4);
  Real H2 = pow(H,2);
  Real H3 = pow(H,3);
  
  Real  drhodp;
  Real  drho_s_dp = 0;
  Real  drho_w_dp = 0;

  Real phij = _phi[_j][_qp];
     
  //   compressed water zone
  if (H < _Hw[_qp])
  {
    if (H >200.0)
      drho_w_dp = E3*((a2*phij)+(a5*phij*H));
  }
  

  // Super heated steam zone
  else if (H > _Hs[_qp])
  {
    drho_s_dp = E3*((b2*phij)-(b3*phij*H)+(b4*4*P3*phij)+(b5*phij*H3));
  }

  // Mixed phase
  else 
  {
    drho_w_dp = E3*((a2*phij)+(a5*phij*_Hw[_qp]));
    drho_s_dp = E3*((b2*phij)-(b3*phij*_Hs[_qp])+(b4*4*P3*phij)+(b5*phij*pow(_Hs[_qp],3)));
  }

  drhodp = (_sat_w[_qp]*drho_w_dp)+(_sat_s[_qp]*drho_s_dp);

  return _porosity[_qp]*_test[_i][_qp]*drhodp/_dt;   
}
