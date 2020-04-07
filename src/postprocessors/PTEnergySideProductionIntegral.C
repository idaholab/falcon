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

Descriptions: calculate P-T based total energy production on a side

----------------------------------------------------------------*/

#include "PTEnergySideProductionIntegral.h"

registerMooseObject("FalconApp", PTEnergySideProductionIntegral);

InputParameters PTEnergySideProductionIntegral::validParams()
{
  InputParameters params = SideIntegralVariablePostprocessor::validParams();

  params.addParam<Real>(
  "injection_temperature", 0.0,
  "Temperature of the injected fluid");

  return params;
}


/*******************************************************************************
Routine: PTEnergySideProductionIntegral --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTEnergySideProductionIntegral::PTEnergySideProductionIntegral(const InputParameters & parameters):
  SideIntegralVariablePostprocessor(parameters),
  _evelo(getMaterialProperty<RealGradient>("energy_convective_velocity")),
  _temp(getParam<Real>("injection_temperature"))
{}


/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergySideProductionIntegral::computeQpIntegral()
{
  return (_u[_qp] - _temp) * _evelo[_qp] * _normals[_qp];
}
