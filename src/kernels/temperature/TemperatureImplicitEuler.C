#include "TemperatureImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

TemperatureImplicitEuler::TemperatureImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),
   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _density_water(getMaterialProperty<Real>("density_water")),
   _density_rock(getMaterialProperty<Real>("density_rock"))
{}

Real
TemperatureImplicitEuler::computeQpResidual()
{
// return 2.677e6*ImplicitEuler::computeQpResidual();
  
Real _const =   

    (
      (
        _porosity[_qp] * _density_water[_qp] * _specific_heat_water[_qp]
      )
      +
      (
        (1.0 - _porosity[_qp]) * _density_rock[_qp] * _specific_heat_rock[_qp]
      )
      );

    
//   std::cout << "time constant =   " <<_time_const<<"\n";

  return  _const*ImplicitEuler::computeQpResidual();

  

//  return (((*_porosity)[_qp]*(*_density_water)[_qp]*(*_specific_heat_water)[_qp])+((1-(*_porosity)[_qp])*(*_density_rock)[_qp]*(*_specific_heat_rock)[_qp]))*ImplicitEuler::computeQpResidual();
}

Real
TemperatureImplicitEuler::computeQpJacobian()
{

//  return 2.677e6*ImplicitEuler::computeQpJacobian();
  
Real _const =   

    (
      (
        _porosity[_qp] * _density_water[_qp] * _specific_heat_water[_qp]
      )
      +
      (
        (1.0 - _porosity[_qp]) * _density_rock[_qp] * _specific_heat_rock[_qp]
      )
      );

    
 return  _const*ImplicitEuler::computeQpJacobian();

//  return (((*_porosity)[_qp]*(*_density_water)[_qp]*(*_specific_heat_water)[_qp])+((1-(*_porosity)[_qp])*(*_density_rock)[_qp]*(*_specific_heat_rock)[_qp]))*ImplicitEuler::computeQpJacobian();
}
