#include "Material.h"
#include "HuyakornEnthalpyConvection.h"

template<>
InputParameters validParams<HuyakornEnthalpyConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

HuyakornEnthalpyConvection::HuyakornEnthalpyConvection(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

   
   _grad_p(coupledGradient("pressure")),
   _dTbydP_H(getMaterialProperty<Real>("dTbydP_H")),
   _Hw(getMaterialProperty<Real>("sat_enthalpy_w")),
   _Hs(getMaterialProperty<Real>("sat_enthalpy_s")),
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity")),
   _darcy_params_s(getMaterialProperty<Real>("darcy_params_s")),
   _darcy_params_w(getMaterialProperty<Real>("darcy_params_w"))
   
{}

Real HuyakornEnthalpyConvection::computeQpResidual()
{

  Real Lamda;
  
  if (_Hw[_qp] <= _u[_qp] && _u[_qp]<=_Hs[_qp])

    {
      Lamda = (_thermal_conductivity[_qp]*_dTbydP_H[_qp])+(_Hw[_qp]*_darcy_params_w[_qp])+(_Hs[_qp]*_darcy_params_s[_qp]);
    }
  else
    {
      Lamda = (_thermal_conductivity[_qp]*_dTbydP_H[_qp])+((_darcy_params_w[_qp]+_darcy_params_s[_qp])*_u[_qp]);
    }
  
   return Lamda* _grad_test[_i][_qp]*_grad_p[_qp];

}

Real HuyakornEnthalpyConvection::computeQpJacobian()
{
  return 0;
 
}
