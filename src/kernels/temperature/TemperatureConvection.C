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
   _darcy_flux_water(getMaterialProperty<RealGradient>("darcy_flux_water")),
   _density_water(getMaterialProperty<Real>("density_water"))
//   _pore_velocity_water(getGradientMaterialProperty("pore_velocity_water"))
{}

Real TemperatureConvection::computeQpResidual()
{

 
  
if(_u[_qp]< 0.0)
  {
  }



// std::cerr<<"darcy_flux_water "<<_darcy_flux_water[_qp]<<".\n";

//   std::cerr<<"water  "<<_density_water[_qp]<<".\n";
//  return _darcy_flux_water_w[_qp]*_phi[_i][_qp]*_grad_u[_qp];

 //std::cerr<<"water  "<<_specific_heat_water[_qp]*_darcy_flux_water[_qp]*_density_water[_qp]*_phi[_i][_qp]*_grad_u[_qp]<<".\n";

  return _specific_heat_water[_qp]*_darcy_flux_water[_qp]*_density_water[_qp]*_phi[_i][_qp]*_grad_u[_qp];
}

Real TemperatureConvection::computeQpJacobian()
{

//   return _darcy_flux_water_w[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
   return _specific_heat_water[_qp]*_darcy_flux_water[_qp]*_density_water[_qp]*_phi[_i][_qp]*_grad_phi[_j][_qp];

}
