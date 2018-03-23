/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "PTPermeabilityAux.h"

registerMooseObject("FalconApp", PTPermeabilityAux);

template<>
InputParameters validParams<PTPermeabilityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

PTPermeabilityAux::PTPermeabilityAux(const InputParameters & parameters):
  AuxKernel(parameters),
  _perm(getMaterialProperty<Real>("permeability"))
{}

Real
PTPermeabilityAux::
computeValue()
{
  return _perm[_qp];
}

