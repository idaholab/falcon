#include "Material.h"
#include "EnthalpyConvectionWater.h"

template<>
InputParameters validParams<EnthalpyConvectionWater>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionWater::EnthalpyConvectionWater(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),

   _darcy_flux_w(getMaterialProperty<RealGradient>("darcy_flux_w")),
   _GHw(getMaterialProperty<RealGradient>("grad_sat_enthalpy_w")),
   _rho_w(getMaterialProperty<Real>("rho_w"))

{}

Real EnthalpyConvectionWater::computeQpResidual()
{
  
   return _darcy_flux_w[_qp]*_rho_w[_qp]*_test[_i][_qp]*_GHw[_qp];

}

Real EnthalpyConvectionWater::computeQpJacobian()
{
  return 0.0;
}
