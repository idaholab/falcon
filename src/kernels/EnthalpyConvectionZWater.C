#include "Material.h"
#include "EnthalpyConvectionZWater.h"

template<>
InputParameters validParams<EnthalpyConvectionZWater>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionZWater::EnthalpyConvectionZWater(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _darcy_params_w(getMaterialProperty<Real>("darcy_params_w")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _rho_w(getMaterialProperty<Real>("rho_w")),
   _Hw(getMaterialProperty<Real>("sat_enthalpy_w")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
EnthalpyConvectionZWater::computeQpResidual()
{
  return _darcy_params_w[_qp]*_Hw[_qp]*_gravity[_qp]*_rho_w[_qp]*_gravity_vector[_qp]*_grad_phi[_i][_qp];
}

Real
EnthalpyConvectionZWater::computeQpJacobian()
{
return 0;
}
