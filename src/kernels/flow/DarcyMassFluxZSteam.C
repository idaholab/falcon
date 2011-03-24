#include "Material.h"
#include "DarcyMassFluxZSteam.h"

template<>
InputParameters validParams<DarcyMassFluxZSteam>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

DarcyMassFluxZSteam::DarcyMassFluxZSteam(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _tau_steam(getMaterialProperty<Real>("tau_steam")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _density_steam(getMaterialProperty<Real>("density_steam")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
DarcyMassFluxZSteam::computeQpResidual()
{
 
  return _tau_steam[_qp]*_gravity[_qp]*_density_steam[_qp]*_gravity_vector[_qp]*_grad_test[_i][_qp];
}

Real
DarcyMassFluxZSteam::computeQpJacobian()
{
return 0;
}
