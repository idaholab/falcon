#include "Flux.h"

template<>
InputParameters validParams<Flux>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("coefficient");
  return params;
}

Flux::Flux(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
   _coefficient(getParam<Real>("coefficient"))
{}

Real
Flux::computeQpIntegral()
{
  return -_coefficient * _grad_u[_qp] * _normals[_qp];
}
