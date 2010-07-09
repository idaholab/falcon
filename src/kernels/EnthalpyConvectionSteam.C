#include "Material.h"
#include "EnthalpyConvectionSteam.h"

template<>
InputParameters validParams<EnthalpyConvectionSteam>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionSteam::EnthalpyConvectionSteam(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

   _darcy_flux_s(getMaterialProperty<RealGradient>("darcy_flux_s")),
   _GHs(getMaterialProperty<RealGradient>("grad_sat_enthalpy_s")),
   _rho_s(getMaterialProperty<Real>("rho_s"))

{}

Real EnthalpyConvectionSteam::computeQpResidual()
{

   mooseAssert(std::!nan(_w[qp]), "darcy_params_w is NAN number");
   mooseAssert(std::!nan(_darcy_params_s[qp]), "darcy_params_s is NAN number");



  return  _darcy_flux_s[_qp]*_rho_s[_qp]*_test[_i][_qp]*_GHs[_qp];
}

Real EnthalpyConvectionSteam::computeQpJacobian()
{

  return 0.0;
  
}
