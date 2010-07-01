#include "Material.h"
#include "EnthalpyConvectionSteam.h"

template<>
InputParameters validParams<EnthalpyConvectionSteam>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

EnthalpyConvectionSteam::EnthalpyConvectionSteam(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _pressure(coupledValue("pressure")),
   _grad_p( coupledGradient("pressure")),
   _darcy_flux_s(getMaterialProperty<RealGradient>("darcy_flux_s")),
   _rho_s(getMaterialProperty<Real>("rho_s"))
{}

Real EnthalpyConvectionSteam::computeQpResidual()
{

//   std::cout << "pressure" << _pressure[_qp]  << " .\n";
  Real P = pow(10,-6)*_pressure[_qp];
  Real GHs;

  GHs =((39.952*pow(P,-2))-(2.54342*2*pow(P,-3))-(0.938879*2*P));  

  Real Resi;
//     Resi = _darcy_flux_s[_qp]*_rho_s[_qp]*_test[_i][_qp]*_grad_p[_qp]*GHs;

//     std::cout << "_grad_p" << _grad_p[_qp] << " .\n";
//     std::cout << "pressure-es" << P << " .\n";
  return  _darcy_flux_s[_qp]*_rho_s[_qp]*_test[_i][_qp]*pow(10,-3)*_grad_p[_qp]*GHs;
}

Real EnthalpyConvectionSteam::computeQpJacobian()
{

  return 0.0;
  
}
