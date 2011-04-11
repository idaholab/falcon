#include "TemperatureTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

TemperatureTimeDerivative::TemperatureTimeDerivative(const std::string & name,
                                                     InputParameters parameters)
  :TimeDerivative(name, parameters),
   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _density_water(getMaterialProperty<Real>("density_water")),
   _density_rock(getMaterialProperty<Real>("density_rock"))
{}

Real
TemperatureTimeDerivative::computeQpResidual()
{
  return  ((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp]) + ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*TimeDerivative::computeQpResidual();
}

Real
TemperatureTimeDerivative::computeQpJacobian()
{
  return ((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp]) + ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*TimeDerivative::computeQpJacobian();
}
