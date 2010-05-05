#include "TemperatureImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

TemperatureImplicitEuler::TemperatureImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters)
{}

void
TemperatureImplicitEuler::subdomainSetup()
{
  _water_specific_heat = &_material->getRealProperty("water_specific_heat");
  _rock_specific_heat = &_material->getRealProperty("rock_specific_heat");
  _porosity = &_material->getRealProperty("porosity");
  _rho_w = &_material->getRealProperty("rho_w");
  _rho_r = &_material->getRealProperty("rho_r");

}

Real
TemperatureImplicitEuler::computeQpResidual()
{
// return 2.677e6*ImplicitEuler::computeQpResidual();
  
Real _const =   

    (
      (
        (*_porosity)[_qp] * (*_rho_w)[_qp] * (*_water_specific_heat)[_qp]
      )
      +
      (
        (1.0 - (*_porosity)[_qp]) * (*_rho_r)[_qp] * (*_rock_specific_heat)[_qp]
      )
      );

    
//   std::cout << "time constant =   " <<_time_const<<"\n";

  return  _const*ImplicitEuler::computeQpResidual();

  

//  return (((*_porosity)[_qp]*(*_rho_w)[_qp]*(*_water_specific_heat)[_qp])+((1-(*_porosity)[_qp])*(*_rho_r)[_qp]*(*_rock_specific_heat)[_qp]))*ImplicitEuler::computeQpResidual();
}

Real
TemperatureImplicitEuler::computeQpJacobian()
{

//  return 2.677e6*ImplicitEuler::computeQpJacobian();
  
Real _const =   

    (
      (
        (*_porosity)[_qp] * (*_rho_w)[_qp] * (*_water_specific_heat)[_qp]
      )
      +
      (
        (1.0 - (*_porosity)[_qp]) * (*_rho_r)[_qp] * (*_rock_specific_heat)[_qp]
      )
      );

    
 return  _const*ImplicitEuler::computeQpJacobian();

//  return (((*_porosity)[_qp]*(*_rho_w)[_qp]*(*_water_specific_heat)[_qp])+((1-(*_porosity)[_qp])*(*_rho_r)[_qp]*(*_rock_specific_heat)[_qp]))*ImplicitEuler::computeQpJacobian();
}
