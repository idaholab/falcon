#include "Material.h"
#include "TemperatureConvection.h"

template<>
InputParameters validParams<TemperatureConvection>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

TemperatureConvection::TemperatureConvection(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _water_specific_heat(getMaterialProperty<Real>("water_specific_heat")),
   _darcy_flux_w(getMaterialProperty<RealGradient>("darcy_flux_w")),
   _rho_w(getMaterialProperty<Real>("rho_w"))
//   _pore_velocity_w(getGradientMaterialProperty("pore_velocity_w"))
{}

Real TemperatureConvection::computeQpResidual()
{

if(_u[_qp]< 0.0)
  {
    
//  std::cout<<"temperature "<<_u[_qp]<<".\n";
//  std::cout<<"darcy_params_w "<<_darcy_params_w[_qp]<<".\n";
  }
      
//  return _darcy_flux_w_w[_qp]*_phi[_i][_qp]*_grad_u[_qp];
  return _water_specific_heat[_qp]*_darcy_flux_w[_qp]*_rho_w[_qp]*_phi[_i][_qp]*_grad_u[_qp];
}

Real TemperatureConvection::computeQpJacobian()
{

//   return _darcy_flux_w_w[_qp]*_phi[_i][_qp]*_dphi[_j][_qp];
   return _water_specific_heat[_qp]*_darcy_flux_w[_qp]*_rho_w[_qp]*_phi[_i][_qp]*_grad_phi[_j][_qp];

}
