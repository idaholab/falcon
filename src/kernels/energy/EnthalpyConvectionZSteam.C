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
   _tau_steam(getMaterialProperty<Real>("tau_steam")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _enthalpy_saturated_steam(getMaterialProperty<Real>("enthalpy_saturated_steam")),
   _density_steam(getMaterialProperty<Real>("density_steam")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
EnthalpyConvectionZSteam::computeQpResidual()
{
 
  return _tau_steam[_qp]*_enthalpy_saturated_steam[_qp]*_gravity[_qp]*_density_steam[_qp]*_gravity_vector[_qp]*_grad_test[_i][_qp];
}

Real
EnthalpyConvectionZSteam::computeQpJacobian()
{
return 0;
}
