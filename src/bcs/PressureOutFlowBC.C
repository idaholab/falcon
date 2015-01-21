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

//******************************************************************************
/*!
  \file    src/bcs/PressurePressureOutFlowBC.C
  \author  Yidong Xia
  \date    October 2014
  \brief   Pressure outflow B.C. in mass balance
 */
//******************************************************************************

#include "PressureOutFlowBC.h"

template<>
InputParameters validParams<PressureOutFlowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  return params;
}

PressureOutFlowBC::PressureOutFlowBC(const std::string & name,
                                   InputParameters parameters) :
  IntegratedBC(name, parameters),
  _tau_water(getMaterialProperty<Real>("tau_water"))
/*******************************************************************************
Routine: PressureBC - constructor
Authors: Yidong Xia
*******************************************************************************/
{}

Real
PressureOutFlowBC::computeQpResidual()
/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
{
  // Should have a negative sign, but that is wrong in computing!
  return _tau_water[_qp]*_grad_u[_qp]*_normals[_qp]*_test[_i][_qp];
}


Real
PressureOutFlowBC::computeQpJacobian()
/*******************************************************************************
Routine: computeQpJacobian
Authors: Yidong Xia
*******************************************************************************/
{
  // Should have a negative sign, but that is wrong in computing!
  return _tau_water[_qp]*_grad_phi[_j][_qp]*_normals[_qp]*_test[_i][_qp];
}
