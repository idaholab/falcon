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
   _water_specific_heat(getMaterialProperty<Real>("water_specific_heat")),
   _rock_specific_heat(getMaterialProperty<Real>("rock_specific_heat")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _rho_w(getMaterialProperty<Real>("rho_w")),
   _rho_r(getMaterialProperty<Real>("rho_r"))
{}

Real
TemperatureImplicitBackwardDifference2::computeQpResidual()
{
  return
    (
      (
        _porosity[_qp] * _rho_w[_qp] * _water_specific_heat[_qp]
      )
      +
      (
        (1.0 - _porosity[_qp]) * _rho_r[_qp] * _rock_specific_heat[_qp]
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
        _porosity[_qp] * _rho_w[_qp] * _water_specific_heat[_qp]
      )
      +
      (
        (1.0 - _porosity[_qp]) * _rho_r[_qp] * _rock_specific_heat[_qp]
      )
    )
    *
  ImplicitBackwardDifference2::computeQpJacobian();

//  return 2.677e6*ImplicitBackwardDifference2::computeQpJacobian();
}
