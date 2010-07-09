#include "Material.h"
#include "EnthalpyConvection.h"

template<>
InputParameters validParams<EnthalpyConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

EnthalpyConvection::EnthalpyConvection(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

   
   _darcy_flux_s(getMaterialProperty<RealGradient>("darcy_flux_s")),
   _darcy_flux_w(getMaterialProperty<RealGradient>("darcy_flux_w")),
   _GHw(getMaterialProperty<RealGradient>("grad_sat_enthalpy_w")),
   _GHs(getMaterialProperty<RealGradient>("grad_sat_enthalpy_s")),
   _rho_w(getMaterialProperty<Real>("rho_w")),
   _rho_s(getMaterialProperty<Real>("rho_s")),
   
   _grad_p(coupledGradient("pressure")),
   _Hw(getMaterialProperty<Real>("sat_enthalpy_w")),
   _Hs(getMaterialProperty<Real>("sat_enthalpy_s")),
   _darcy_params_s(getMaterialProperty<Real>("darcy_params_s")),
   _darcy_params_w(getMaterialProperty<Real>("darcy_params_w"))
   
{




}

Real EnthalpyConvection::computeQpResidual()
{
  /*  Real H =_u[_qp]* 1e-3;
   if (_Hw[_qp]<=H<=_Hs[_qp])
   {
      Real conve;

      conve = _test[_i][_qp]*((_darcy_flux_s[_qp]*_rho_s[_qp]*_GHs[_qp])+(_darcy_flux_w[_qp]*_rho_w[_qp]*_GHw[_qp]));

     // std::cout<<conve<<"conve"<<".\n";
     
      
     return  _test[_i][_qp]*((_darcy_flux_s[_qp]*_rho_s[_qp]*_GHs[_qp])+(_darcy_flux_w[_qp]*_rho_w[_qp]*_GHw[_qp]));
   }
   else 
   {
     return _test[_i][_qp]*_grad_u[_qp]*((_darcy_flux_s[_qp]*_rho_s[_qp])+(_darcy_flux_w[_qp]*_rho_w[_qp]));
   }
  */

   
   return ((_Hs[_qp]*_darcy_params_s[_qp])+(_Hw[_qp]*_darcy_params_w[_qp]))* _grad_test[_i][_qp]*_grad_p[_qp];
   

}

Real EnthalpyConvection::computeQpJacobian()
{

  return 0;
  
//  return  _test[_i][_qp]*_grad_phi[_j][_qp]*((_darcy_flux_s[_qp]*_rho_s[_qp])+(_darcy_flux_w[_qp]*_rho_w[_qp]));;
  
}
