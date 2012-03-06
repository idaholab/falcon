/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

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
