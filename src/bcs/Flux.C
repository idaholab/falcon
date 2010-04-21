#include "Flux.h"

template<>
InputParameters validParams<Flux>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("coefficient");
  return params;
}

Flux::Flux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
   _coefficient(parameters.get<Real>("coefficient"))
{}

Real
Flux::computeQpIntegral()
{
  return -_coefficient * _grad_u_face[_qp] * _normals_face[_qp];
}
