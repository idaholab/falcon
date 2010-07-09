
#include "CoupledDarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<CoupledDarcyImplicitEuler>()
{
     InputParameters params = validParams<Kernel>();
     params.addCoupledVar("enthalpy", "TODO: add description");
     params.addCoupledVar("rhoAux", "TODO: add description");
     return params;
}

CoupledDarcyImplicitEuler::CoupledDarcyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

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
     a1 = 4.00894*pow(10,-4);
     a2 = 5.46283*pow(10,-7);
     b1 = 0.0438441;
     b2 = 1.79088*pow(10,-5);
     b3 = 3.69276*pow(10,-8);
     b4 = 5.17644*pow(10,-13);
}

Real
CoupledDarcyImplicitEuler::computeQpResidual()
{ 
    
     return _porosity[_qp]*_test[_i][_qp]*(_rho[_qp] -_rho_old[_qp])/_dt;
}

Real
CoupledDarcyImplicitEuler::computeQpJacobian()
{
    
     
     Real H = _enthalpy[_qp]/ E3;
     Real P = _u[_qp] /E6;
     Real P3= pow(P,3);
     Real  _drho;
     Real  _drho_s;
     Real  _drho_w;
     
//   compressed water zone
     if ( H < _Hw[_qp])
     {
     _drho = E3*((a1*_phi[_j][_qp])+(a2*_phi[_j][_qp]*H));
     }

//   super heated steam zone
     else if (H > _Hs[_qp])
     {
     _drho = E3*((b1*_phi[_j][_qp])-(b2*_phi[_j][_qp]*H)+(b3*4*P3*_phi[_j][_qp])+(b4*_phi[_j][_qp]*pow(H,3)));
     }

//   Mixed Phase Zone(Two-phase exists)
     else 
     {
     _drho_w = E3*((a1*_phi[_j][_qp])+(a2*_phi[_j][_qp]*_Hw[_qp]));
     _drho_s = E3*((b1*_phi[_j][_qp])-(b2*_phi[_j][_qp]*_Hs[_qp])+(b3*4*P3*_phi[_j][_qp])+(b4*_phi[_j][_qp]*pow(_Hs[_qp],3)));
     _drho = (_sat_w[_qp] * _drho_w)+((_sat_s[_qp])*_drho_s);
     }
     return _porosity[_qp]*_test[_i][_qp]*(_drho)/_dt;


//     return 0;
     
}
