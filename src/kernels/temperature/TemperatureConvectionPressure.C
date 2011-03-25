#include "Material.h"
#include "TemperatureConvectionPressure.h"

template<>
InputParameters validParams<TemperatureConvectionPressure>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TemperatureConvectionPressure::TemperatureConvectionPressure(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _darcy_mass_flux_water_pressure(getMaterialProperty<RealGradient>("darcy_mass_flux_water_pressure")),
   _density_water(getMaterialProperty<Real>("density_water"))
   
//   _pore_velocity_water(getGradientMaterialProperty("pore_velocity_water"))
{}

Real TemperatureConvectionPressure::computeQpResidual()
{


 return _specific_heat_water[_qp]*_darcy_mass_flux_water_pressure[_qp]*_test[_i][_qp]*_grad_u[_qp];

}

Real TemperatureConvectionPressure::computeQpJacobian()
{

//   return _darcy_flux_water_w[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
   return _specific_heat_water[_qp]*_darcy_mass_flux_water_pressure[_qp]*_test[_i][_qp]*_grad_phi[_j][_qp];

}

