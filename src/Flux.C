#include "Flux.h"

template<>
InputParameters validParams<Flux>()
{
  InputParameters params;
  params.set<Real>("coefficient");
  return params;
}

Flux::Flux(std::string name, InputParameters parameters, std::string var_name, unsigned int boundary_id, std::vector<std::string> coupled_to, std::vector<std::string> coupled_as)
  :BoundaryCondition(name, parameters, var_name, true, boundary_id, coupled_to, coupled_as),
   _coefficient(parameters.get<Real>("coefficient"))
{}

Real
Flux::computeQpIntegral()
{
  return -_coefficient * _grad_u_face[_qp] * _normals_face[_qp];
}
