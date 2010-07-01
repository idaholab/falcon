#include "Material.h"
#include "EnthalpyConvectionWater.h"

template<>
InputParameters validParams<EnthalpyConvectionWater>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

EnthalpyConvectionWater::EnthalpyConvectionWater(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

   _pressure( coupledValue("pressure")),
   _grad_p( coupledGradient("pressure")),
   _darcy_flux_w(getMaterialProperty<RealGradient>("darcy_flux_w")),
   _rho_w(getMaterialProperty<Real>("rho_w"))

{}

Real EnthalpyConvectionWater::computeQpResidual()
{
  // std::cout << "pressure_before" << _p[_qp] << " .\n";
  Real P = pow(10,-6)*_pressure[_qp];
  
  
  Real GHw;
  //std::cout << "pressure" << _p[_qp] << " .\n";
   GHw = (94.465)-(4.502*2*P)+(0.120265*3*pow(P,2))+(162.7*pow(P,-2))-(29.8163*2*pow(P,-3))+(1.75623*3*pow(P,-4));
    Real Resi;
    // Resi = _darcy_flux_w[_qp]*_rho_w[_qp]*_test[_i][_qp]*_grad_p[_qp]*GHw;

    //    std::cout << "GHw" << GHw << " .\n";
    //  std::cout << "grad_p" << _grad_p[_qp] << " .\n";
   return _darcy_flux_w[_qp]*_rho_w[_qp]*_test[_i][_qp]*pow(10,-3)*_grad_p[_qp]*GHw;

}

Real EnthalpyConvectionWater::computeQpJacobian()
{
  return 0.0;
}
