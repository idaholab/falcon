#include "Material.h"
#include "DarcyVelocity.h"
 
template<>
InputParameters validParams<DarcyVelocity>()
{
  InputParameters params = validParams<Kernel>();
  params.set<int>("component")= 0;
  
  return params;
}

DarcyVelocity::DarcyVelocity(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _component(parameters.get<int>("component"))
{}

void
DarcyVelocity::subdomainSetup()
{
  _pore_velocity = &_material->getGradientProperty("pore_velocity");
}

Real
DarcyVelocity::computeQpResidual()
{
  return ((_u[_qp]-( *_pore_velocity)[_qp](_component) )) *_phi[_i][_qp];
}


Real
DarcyVelocity::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

