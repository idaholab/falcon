#include "Material.h"
#include "TemperatureConvection.h"

template<>
InputParameters validParams<TemperatureConvection>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TemperatureConvection::TemperatureConvection(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _water_specific_heat(getRealMaterialProperty("water_specific_heat")),
   _darcy_flux(getGradientMaterialProperty("darcy_flux")),
   _rho_w(getRealMaterialProperty("rho_w"))
//   _pore_velocity(getGradientMaterialProperty("pore_velocity"))
{}

Real TemperatureConvection::computeQpResidual()
{

//  return _darcy_flux[_qp]*_phi[_i][_qp]*_grad_u[_qp];
  return _water_specific_heat[_qp]*_darcy_flux[_qp]*_rho_w[_qp]*_phi[_i][_qp]*_grad_u[_qp];
}

Real TemperatureConvection::computeQpJacobian()
{
//   return _darcy_flux[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
   return _water_specific_heat[_qp]*_darcy_flux[_qp]*_rho_w[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
}
