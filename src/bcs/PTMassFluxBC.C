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

Descriptions: P-T based single-phase mass balance equation
              fixed mass flux BC normal to the face

----------------------------------------------------------------*/

#include "PTMassFluxBC.h"

registerMooseObject("FalconApp", PTMassFluxBC);

InputParameters PTMassFluxBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();

  params.addRequiredParam<Real>(
  "value",
  "User-input mass flux value at normal to the boundary face");

  return params;
}


/*******************************************************************************
Routine: PTMassFluxBC --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTMassFluxBC::PTMassFluxBC(const InputParameters & parameters):
  IntegratedBC(parameters),
  _value(getParam<Real>("value"))
{}


/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassFluxBC::
computeQpResidual()
{
  return -_value*_test[_i][_qp];
}


/*******************************************************************************
Routine: computeQpJacobian
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassFluxBC::
computeQpJacobian()
{
  return 0.0;
}
