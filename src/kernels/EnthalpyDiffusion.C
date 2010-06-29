#include "Material.h"
#include "EnthalpyDiffusion.h"

template<>
InputParameters validParams<EnthalpyDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  params.addCoupledVar("pressure", "TODO: add description");
  params.addCoupledVar("tempAux", "TODO: add description");
  return params;
}

EnthalpyDiffusion::EnthalpyDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
   _grad_p( coupledGrad("pressure")),
   _grad_T( coupledGrad("tempAux")),
   _dTbydP_H(getRealMaterialProperty("dTbydP_H")),
   _dTbydH_P(getRealMaterialProperty("dTbydH_P")),
   _thermal_conductivity(getRealMaterialProperty("thermal_conductivity"))
{}

Real
EnthalpyDiffusion::computeQpResidual()
{
  Real Resi;
  Resi = -_thermal_conductivity[_qp]*((_dTbydP_H[_qp]*_grad_p[_qp]*_grad_test[_i][_qp])+(_dTbydH_P[_qp]*Diffusion::computeQpResidual()));

  //std::cout << "ResiEnth4" << Resi << " .\n";
//  return -_thermal_conductivity[_qp]*((_dTbydP_H[_qp]*_grad_p[_qp]*_grad_test[_i][_qp])-(_dTbydH_P[_qp]*Diffusion::computeQpResidual()));
  return _thermal_conductivity[_qp]*(_grad_T[_qp]*_grad_test[_i][_qp]);
  
}

Real
EnthalpyDiffusion::computeQpJacobian()
{
  return 0;
  
  // return _thermal_conductivity[_qp]*_dTbydH_P[_qp] * Diffusion::computeQpJacobian();
}
