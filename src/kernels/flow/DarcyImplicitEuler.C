#include "DarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<DarcyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

DarcyImplicitEuler::DarcyImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),
   _compressibility(getMaterialProperty<Real>("compressibility")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _density_water(getMaterialProperty<Real>("density_water"))
{}

Real
DarcyImplicitEuler::computeQpResidual()
{
  return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitEuler::computeQpResidual();
}

Real
DarcyImplicitEuler::computeQpJacobian()
{
  return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitEuler::computeQpJacobian();
}
