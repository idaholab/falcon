#include "Flux.h"

template<>
InputParameters validParams<Flux>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.set<Real>("coefficient");
  return params;
}

Flux::Flux(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
   _coefficient(getParam<Real>("coefficient"))
{}

Real
Flux::computeQpIntegral()
{
  return -_coefficient * _grad_u[_qp] * _normals[_qp];
}
