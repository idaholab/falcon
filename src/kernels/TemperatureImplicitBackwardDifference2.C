#include "TemperatureImplicitBackwardDifference2.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureImplicitBackwardDifference2>()
{
  InputParameters params = validParams<ImplicitBackwardDifference2>();
  return params;
}

TemperatureImplicitBackwardDifference2::TemperatureImplicitBackwardDifference2(std::string name,
                                                                               MooseSystem & moose_system,
                                                                               InputParameters parameters)
  :ImplicitBackwardDifference2(name, moose_system, parameters)
{}

void
TemperatureImplicitBackwardDifference2::subdomainSetup()
{
  _water_specific_heat = &_material->getRealProperty("water_specific_heat");
  _rock_specific_heat = &_material->getRealProperty("rock_specific_heat");
  _porosity = &_material->getRealProperty("porosity");
  _rho_w = &_material->getRealProperty("rho_w");
  _rho_r = &_material->getRealProperty("rho_r");

}

Real
TemperatureImplicitBackwardDifference2::computeQpResidual()
{
  return
    (
      (
        (*_porosity)[_qp] * (*_rho_w)[_qp] * (*_water_specific_heat)[_qp]
      )
      +
      (
        (1.0 - (*_porosity)[_qp]) * (*_rho_r)[_qp] * (*_rock_specific_heat)[_qp]
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
        (*_porosity)[_qp] * (*_rho_w)[_qp] * (*_water_specific_heat)[_qp]
      )
      +
      (
        (1.0 - (*_porosity)[_qp]) * (*_rho_r)[_qp] * (*_rock_specific_heat)[_qp]
      )
    )
    *
  ImplicitBackwardDifference2::computeQpJacobian();

//  return 2.677e6*ImplicitBackwardDifference2::computeQpJacobian();
}
