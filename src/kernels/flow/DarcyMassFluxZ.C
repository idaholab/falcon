#include "Material.h"
#include "DarcyMassFluxZ.h"

template<>
InputParameters validParams<DarcyMassFluxZ>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

DarcyMassFluxZ::DarcyMassFluxZ(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _tau_water(getMaterialProperty<Real>("tau_water")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _density_water(getMaterialProperty<Real>("density_water")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
DarcyMassFluxZ::computeQpResidual()
{
  return _tau_water[_qp]*_gravity[_qp]*_density_water[_qp]*_gravity_vector[_qp]*_grad_phi[_i][_qp];
}

Real
DarcyMassFluxZ::computeQpJacobian()
{
return 0;
}
