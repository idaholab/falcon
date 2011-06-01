#include "Material.h"
#include "EnthalpyDiffusion.h"

template<>
InputParameters validParams<EnthalpyDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
 // params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("temperature", "TODO: add description");
  params.addCoupledVar("dTdH_P", "TODO: add description");   
  return params;
}

EnthalpyDiffusion::EnthalpyDiffusion(const std::string & name, InputParameters parameters)
  :Diffusion(name, parameters),
   
    _grad_T(coupledGradient("temperature")),
    _dTdH_P(coupledValue("dTdH_P")),

//   _pressure(coupledValue("pressure")),
//   _grad_p(coupledGradient("pressure")),
//   _dTbydP_H(getMaterialProperty<Real>("dTbydP_H")),
//   _dTbydH_P(getMaterialProperty<Real>("dTbydH_P")),
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
EnthalpyDiffusion::computeQpResidual()
{

   
 //  return _thermal_conductivity[_qp]*((_dTbydP_H[_qp]*_grad_p[_qp]*_grad_test[_i][_qp])+(_dTbydH_P[_qp]*Diffusion::computeQpResidual()));

     return  _thermal_conductivity[_qp]*(_grad_T[_qp]*_grad_test[_i][_qp]);
 // return _thermal_conductivity[_qp]* _dTdH_P[_qp] * Diffusion::computeQpResidual();
}

Real
EnthalpyDiffusion::computeQpJacobian()
{
  //return 0;
   return _thermal_conductivity[_qp]*_dTdH_P[_qp] *  Diffusion::computeQpJacobian();
   
}
