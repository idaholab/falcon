#include "Material.h"
#include "StrainCompute.h"
 
template<>
InputParameters validParams<StrainCompute>()
{
  InputParameters params;
  params.set<int>("component")= 0;
  
  return params;
}

StrainCompute::StrainCompute(std::string name,
                  InputParameters parameters,
                  std::string var_name,
                  std::vector<std::string> coupled_to,
                  std::vector<std::string> coupled_as)
    :Kernel(name,parameters,var_name,true,coupled_to,coupled_as),
     _component(parameters.get<int>("component"))
{}

void
StrainCompute::subdomainSetup()
{
  _stress_normal_vector = &_material->getRealVectorValueProperty("stress_normal_vector");
  _stress_shear_vector  = &_material->getRealVectorValueProperty("stress_shear_vector");
}

Real
StrainCompute::computeQpResidual()
{
  if ( _component <= 2)  
    return (_u[_qp]-(*_stress_normal_vector)[_qp](_component)) * _phi[_i][_qp];
  else
    return (_u[_qp]-(*_stress_shear_vector)[_qp](_component-3)) * _phi[_i][_qp];
  
}


Real
StrainCompute::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

