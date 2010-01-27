#include "Material.h"
#include "StressCompute.h"
 
template<>
InputParameters validParams<StressCompute>()
{
  InputParameters params;
  params.set<int>("component")= 0;
  
  return params;
}

StressCompute::StressCompute(std::string name,
                  InputParameters parameters,
                  std::string var_name,
                  std::vector<std::string> coupled_to,
                  std::vector<std::string> coupled_as)
    :Kernel(name,parameters,var_name,true,coupled_to,coupled_as),
     _component(parameters.get<int>("component"))
{}

void
StressCompute::subdomainSetup()
{
  _stress_normal_vector = &_material->getRealVectorValueProperty("stress_normal_vector");
  _stress_shear_vector  = &_material->getRealVectorValueProperty("stress_shear_vector");
}

Real
StressCompute::computeQpResidual()
{
  if ( _component <= 2)  
    return (_u[_qp]-(*_stress_normal_vector)[_qp](_component)) * _phi[_i][_qp];
  else
    return (_u[_qp]-(*_stress_shear_vector)[_qp](_component-3)) * _phi[_i][_qp];
  
}


Real
StressCompute::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

