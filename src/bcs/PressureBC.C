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
  \file    src/bcs/PressureBC.C
  \author  Yidong Xia 
  \date    October 2014
  \brief   Specify external pressure
 */
//******************************************************************************

#include "PressureBC.h"

template<>
InputParameters validParams<PressureBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<unsigned int>("component", "Component");
  params.addRequiredParam<Real>("pressure", "Specify pressure");
  params.addParam<Real>("alpha", 1.0, "Biot effective stress coefficient. Default = 1.0");

  return params;
}

PressureBC::PressureBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  _component(getParam<unsigned int>("component")),
  _pressure(getParam<Real>("pressure")),
  _alpha(getParam<Real>("alpha"))
/*******************************************************************************
Routine: PressureBC - constructor
Authors: Yidong Xia
*******************************************************************************/
{}

Real
PressureBC::computeQpResidual()
/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
{
  if(_component > 2) mooseError("Unknown pressure component");
  return _alpha * _pressure * _normals[_qp](_component) * _test[_i][_qp];
}
