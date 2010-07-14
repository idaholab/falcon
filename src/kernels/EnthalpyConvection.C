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

   
   _grad_p(coupledGradient("pressure")),
   _Hw(getMaterialProperty<Real>("sat_enthalpy_w")),
   _Hs(getMaterialProperty<Real>("sat_enthalpy_s")),
   _darcy_params_s(getMaterialProperty<Real>("darcy_params_s")),
   _darcy_params_w(getMaterialProperty<Real>("darcy_params_w"))
   
{}

Real EnthalpyConvection::computeQpResidual()
{
  
  return ((_Hs[_qp]*_darcy_params_s[_qp])+(_Hw[_qp]*_darcy_params_w[_qp]))* _grad_test[_i][_qp]*_grad_p[_qp];

}

Real EnthalpyConvection::computeQpJacobian()
{
  return 0;
 
}
