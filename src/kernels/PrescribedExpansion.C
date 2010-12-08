#include "PrescribedExpansion.h"

template<>
InputParameters validParams<PrescribedExpansion>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.set<Real>("component");
  params.set<Real>("percent");
  return params;
}

PrescribedExpansion::PrescribedExpansion(const std::string & name, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, parameters),
   _component(getParam<Real>("component")),
   _percent(getParam<Real>("percent"))
{}

Real
PrescribedExpansion::computeQpResidual()
{
  recomputeCouplingConstants();
  
  return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](_component)*0.01*_percent/3.0);
}
