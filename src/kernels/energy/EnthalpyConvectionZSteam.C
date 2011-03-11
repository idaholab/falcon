#include "Material.h"
#include "EnthalpyConvectionZSteam.h"

template<>
InputParameters validParams<EnthalpyConvectionZSteam>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionZSteam::EnthalpyConvectionZSteam(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _darcy_params_s(getMaterialProperty<Real>("darcy_params_s")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _Hs(getMaterialProperty<Real>("sat_enthalpy_s")),
   _rho_s(getMaterialProperty<Real>("rho_s")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
EnthalpyConvectionZSteam::computeQpResidual()
{
 
  return _darcy_params_s[_qp]*_Hs[_qp]*_gravity[_qp]*_rho_s[_qp]*_gravity_vector[_qp]*_grad_test[_i][_qp];
}

Real
EnthalpyConvectionZSteam::computeQpJacobian()
{
return 0;
}
