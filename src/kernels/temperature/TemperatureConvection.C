#include "Material.h"
#include "TemperatureConvection.h"

template<>
InputParameters validParams<TemperatureConvection>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TemperatureConvection::TemperatureConvection(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water"))

{}

Real TemperatureConvection::computeQpResidual()
{

  return _specific_heat_water[_qp]*_darcy_mass_flux_water[_qp]*_test[_i][_qp]*_grad_u[_qp];

}

Real TemperatureConvection::computeQpJacobian()
{


   return _specific_heat_water[_qp]*_darcy_mass_flux_water[_qp]*_test[_i][_qp]*_grad_phi[_j][_qp];

}

