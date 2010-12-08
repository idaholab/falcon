#include "Material.h"
#include "EnthalpyConvectionSteam.h"

template<>
InputParameters validParams<EnthalpyConvectionSteam>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionSteam::EnthalpyConvectionSteam(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),

   _darcy_flux_s(getMaterialProperty<RealGradient>("darcy_flux_s")),
   _GHs(getMaterialProperty<RealGradient>("grad_sat_enthalpy_s")),
   _rho_s(getMaterialProperty<Real>("rho_s"))

{}

Real EnthalpyConvectionSteam::computeQpResidual()
{


  return  _darcy_flux_s[_qp]*_rho_s[_qp]*_test[_i][_qp]*_GHs[_qp];
}

Real EnthalpyConvectionSteam::computeQpJacobian()
{

  return 0.0;
  
}
