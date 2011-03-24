#include "DarcyImplicitBackwardDifference2.h"
#include "Material.h"

template<>
InputParameters validParams<DarcyImplicitBackwardDifference2>()
{
  InputParameters params = validParams<ImplicitBackwardDifference2>();
  return params;
}

DarcyImplicitBackwardDifference2::DarcyImplicitBackwardDifference2(const std::string & name,
                                                                   InputParameters parameters)
  :ImplicitBackwardDifference2(name, parameters),
   _compressibility(getMaterialProperty<Real>("compressibility")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _density_water(getMaterialProperty<Real>("density_water"))
{}

Real
DarcyImplicitBackwardDifference2::computeQpResidual()
{
  return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitBackwardDifference2::computeQpResidual();
}

Real
DarcyImplicitBackwardDifference2::computeQpJacobian()
{
  return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitBackwardDifference2::computeQpJacobian();
}
