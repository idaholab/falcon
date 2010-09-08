#include "Material.h"
#include "StrainCompute.h"
 
template<>
InputParameters validParams<StrainCompute>()
{
  InputParameters params = validParams<Kernel>();
  params.set<int>("component")= 0;
  
  return params;
}

StrainCompute::StrainCompute(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _component(getParam<int>("component")),
   _stress_normal_vector(getMaterialProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector(getMaterialProperty<RealVectorValue>("stress_shear_vector"))
{}

Real
StrainCompute::computeQpResidual()
{
  if ( _component <= 2)  
    return (_u[_qp]-_stress_normal_vector[_qp](_component)) * _phi[_i][_qp];
  else
    return (_u[_qp]-_stress_shear_vector[_qp](_component-3)) * _phi[_i][_qp];
  
}

Real
StrainCompute::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

