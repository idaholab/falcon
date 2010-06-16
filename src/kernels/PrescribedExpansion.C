#include "PrescribedExpansion.h"

template<>
InputParameters validParams<PrescribedExpansion>()
{
  InputParameters params = validParams<SolidMechTempCouple>();
  params.set<Real>("component");
  params.set<Real>("percent");
  return params;
}

PrescribedExpansion::PrescribedExpansion(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechTempCouple(name, moose_system, parameters),
   _component(parameters.get<Real>("component")),
   _percent(parameters.get<Real>("percent"))
{}

Real
PrescribedExpansion::computeQpResidual()
{
  recomputeCouplingConstants();
  
  return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](_component)*0.01*_percent/3.0);
}
