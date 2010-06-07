#include "Flux.h"

template<>
InputParameters validParams<Flux>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("coefficient");
  return params;
}

Flux::Flux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, setIntegratedParam(parameters, true)),
   _coefficient(parameters.get<Real>("coefficient"))
{}

Real
Flux::computeQpIntegral()
{
  return -_coefficient * _grad_u[_qp] * _normals[_qp];
}
