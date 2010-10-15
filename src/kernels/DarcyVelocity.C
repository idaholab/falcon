#include "Material.h"
#include "DarcyVelocity.h"
 
template<>
InputParameters validParams<DarcyVelocity>()
{
  InputParameters params = validParams<Kernel>();
  params.set<int>("component")= 0;
  
  return params;
}

DarcyVelocity::DarcyVelocity(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _component(getParam<int>("component")),
   _pore_velocity_w(getMaterialProperty<RealGradient>("pore_velocity_w"))

   
{}

Real
DarcyVelocity::computeQpResidual()
{

  return ((_u[_qp]-_pore_velocity_w[_qp](_component) )) *_phi[_i][_qp];
  
 
}


Real
DarcyVelocity::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

