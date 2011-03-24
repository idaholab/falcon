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
   _tau_water(getMaterialProperty<Real>("tau_water")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _density_water(getMaterialProperty<Real>("density_water")),
   _enthalpy_saturated_water(getMaterialProperty<Real>("enthalpy_saturated_water")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
EnthalpyConvectionZWater::computeQpResidual()
{
  return _tau_water[_qp]*_enthalpy_saturated_water[_qp]*_gravity[_qp]*_density_water[_qp]*_gravity_vector[_qp]*_grad_phi[_i][_qp];
}

Real
EnthalpyConvectionZWater::computeQpJacobian()
{
return 0;
}
