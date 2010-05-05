#include "Material.h"
#include "TemperatureConvection.h"

template<>
InputParameters validParams<TemperatureConvection>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TemperatureConvection::TemperatureConvection(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters)
{}

void
TemperatureConvection::subdomainSetup()
{
   _water_specific_heat = &_material->getRealProperty("water_specific_heat");
   _darcy_flux =  &_material->getGradientProperty("darcy_flux");
   _rho_w = &_material->getRealProperty("rho_w");
//   _pore_velocity =  &_material->getGradientProperty("pore_velocity");
}

Real TemperatureConvection::computeQpResidual()
{

//  return (*_darcy_flux)[_qp]*_phi[_i][_qp]*_grad_u[_qp];
  return (*_water_specific_heat)[_qp]*(*_darcy_flux)[_qp]*(*_rho_w)[_qp]*_phi[_i][_qp]*_grad_u[_qp];
}

Real TemperatureConvection::computeQpJacobian()
{
//   return (*_darcy_flux)[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
   return (*_water_specific_heat)[_qp]*(*_darcy_flux)[_qp]*(*_rho_w)[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
}
