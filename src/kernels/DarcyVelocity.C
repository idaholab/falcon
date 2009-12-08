#include "Material.h"
#include "DarcyVelocity.h"
 
template<>
InputParameters validParams<DarcyVelocity>()
{
  InputParameters params;
  params.set<int>("component")= 0;
  
  return params;
}

DarcyVelocity::DarcyVelocity(std::string name,
                  InputParameters parameters,
                  std::string var_name,
                  std::vector<std::string> coupled_to,
                  std::vector<std::string> coupled_as)
    :Kernel(name,parameters,var_name,true,coupled_to,coupled_as),
     _component(parameters.get<int>("component"))
     
{}

void
DarcyVelocity::subdomainSetup()
{
  _darcy_velocity = &_material->getGradientProperty("darcy_velocity");
}

Real
DarcyVelocity::computeQpResidual()
{
  return ((_u[_qp]-( *_darcy_velocity)[_qp](_component) )) *_phi[_i][_qp];
}


Real
DarcyVelocity::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

