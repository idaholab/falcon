#include "Material.h"
#include "HuyakornMassConvection.h"

template<>
InputParameters validParams<HuyakornMassConvection>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

HuyakornMassConvection::HuyakornMassConvection(std::string name,
                                             MooseSystem & moose_system,
                                             InputParameters parameters)
  :Diffusion(name, moose_system, parameters),
    _darcy_params_w(getMaterialProperty<Real>("darcy_params_w")),
   _darcy_params_s(getMaterialProperty<Real>("darcy_params_s"))
{}

Real
HuyakornMassConvection::computeQpResidual()
{
  Real tau ;
  
   tau = _darcy_params_w[_qp]+_darcy_params_s[_qp]; 

//   std::cout<<"tau"<<tau<<".\n";
   
  return tau*Diffusion::computeQpResidual();
}

Real
HuyakornMassConvection::computeQpJacobian()
{
  Real tau ;
  
   tau = _darcy_params_w[_qp]+_darcy_params_s[_qp]; 
 
  return tau*Diffusion::computeQpJacobian();
}
