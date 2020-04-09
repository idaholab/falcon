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

/*----------------------------------------------------------------

Contributors: Yidong Xia (INL)

Descriptions: output computed Darcy flux

----------------------------------------------------------------*/

#include "PTFluidVelocityAux.h"

registerMooseObject("FalconApp", PTFluidVelocityAux);

InputParameters PTFluidVelocityAux::validParams()
{
  InputParameters params = PTDarcyFluxAux::validParams();
  return params;
}


/*******************************************************************************
Routine: PTFluidVelocityAux -- constructor
Authors: Yidong Xia
*******************************************************************************/
PTFluidVelocityAux::PTFluidVelocityAux(const InputParameters & parameters):
  PTDarcyFluxAux(parameters),
  _poro(getMaterialProperty<Real>("porosity"))
{}


/*******************************************************************************
Routine: computeValue
Authors: Yidong Xia
*******************************************************************************/
Real
PTFluidVelocityAux::
computeValue()
{
  return PTDarcyFluxAux::computeValue() / _poro[_qp];
}
