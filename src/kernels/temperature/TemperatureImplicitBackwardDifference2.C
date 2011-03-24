#include "TemperatureImplicitBackwardDifference2.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureImplicitBackwardDifference2>()
{
  InputParameters params = validParams<ImplicitBackwardDifference2>();
  return params;
}

TemperatureImplicitBackwardDifference2::TemperatureImplicitBackwardDifference2(const std::string & name,
                                                                               InputParameters parameters)
  :ImplicitBackwardDifference2(name, parameters),
   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _density_water(getMaterialProperty<Real>("density_water")),
   _density_rock(getMaterialProperty<Real>("density_rock"))
{}

Real
TemperatureImplicitBackwardDifference2::computeQpResidual()
{
  return
    (
      (
        _porosity[_qp] * _density_water[_qp] * _specific_heat_water[_qp]
      )
      +
      (
        (1.0 - _porosity[_qp]) * _density_rock[_qp] * _specific_heat_rock[_qp]
      )
    )
    *
    ImplicitBackwardDifference2::computeQpResidual();
  
//  return 2.677e6*ImplicitBackwardDifference2::computeQpResidual();
}

Real
TemperatureImplicitBackwardDifference2::computeQpJacobian()
{
return
    (
      (
        _porosity[_qp] * _density_water[_qp] * _specific_heat_water[_qp]
      )
      +
      (
        (1.0 - _porosity[_qp]) * _density_rock[_qp] * _specific_heat_rock[_qp]
      )
    )
    *
  ImplicitBackwardDifference2::computeQpJacobian();

//  return 2.677e6*ImplicitBackwardDifference2::computeQpJacobian();
}
