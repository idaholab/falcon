#include "Material.h"
#include "StressCompute.h"
 
template<>
InputParameters validParams<StressCompute>()
{
  InputParameters params = validParams<Kernel>();
  params.set<int>("component")= 0;
  
  return params;
}

StressCompute::StressCompute(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _component(getParam<int>("component")),
   _stress_normal_vector(getMaterialProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector(getMaterialProperty<RealVectorValue>("stress_shear_vector"))
{}

Real
StressCompute::computeQpResidual()
{
  if ( _component <= 2)  
    return (_u[_qp]-_stress_normal_vector[_qp](_component)) * _phi[_i][_qp];
  else
    return (_u[_qp]-_stress_shear_vector[_qp](_component-3)) * _phi[_i][_qp];
}

Real
StressCompute::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

