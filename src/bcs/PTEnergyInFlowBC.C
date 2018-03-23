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

Descriptions: P-T based single-phase energy balance equation
              outflow boundary condition

----------------------------------------------------------------*/

#include "PTEnergyInFlowBC.h"

registerMooseObject("FalconApp", PTEnergyInFlowBC);

template<>
InputParameters validParams<PTEnergyInFlowBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  params.addRequiredParam<Real>(
  "value",
  "User-input temperature value at boundary");

  return params;
}


/*******************************************************************************
Routine: PTEnergyInFlowBC --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTEnergyInFlowBC::PTEnergyInFlowBC(const InputParameters & parameters):
  IntegratedBC(parameters),
  _thco(getMaterialProperty<Real>("thermal_conductivity")),
  _evelo(getMaterialProperty<RealGradient>("energy_convective_velocity")),
  _value(getParam<Real>("value"))
{}


/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyInFlowBC::
computeQpResidual()
{
  return
  (-_thco[_qp]*_grad_u[_qp]+_evelo[_qp]*_value)*_test[_i][_qp]*_normals[_qp];
}


/*******************************************************************************
Routine: computeQpJacobian
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyInFlowBC::
computeQpJacobian()
{
  return 0.0;
}
