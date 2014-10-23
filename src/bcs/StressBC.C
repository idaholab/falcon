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
  \file    src/bcs/StressBC.C
  \author  Yidong Xia 
  \date    October 2014
  \brief   Specify external stress
 */
//******************************************************************************

#include "StressBC.h"

template<>
InputParameters validParams<StressBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<unsigned int>("component", "Component");
  params.addRequiredParam<Real>("pressure", "Specify stress");
  params.addParam<Real>("alpha", 1.0, "Biot effective stress coefficient. Default = 1.0");

  return params;
}

StressBC::StressBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  _component(getParam<unsigned int>("component")),
  _pressure(getParam<Real>("pressure")),
  _alpha(getParam<Real>("alpha"))
/*******************************************************************************
Routine: StressBC - constructor
Authors: Yidong Xia
*******************************************************************************/
{}

Real
StressBC::computeQpResidual()
/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
{
    if(_component == 0)
      return -_alpha * _pressure * _normals[_qp](0);
    else if(_component == 1)
      return -_alpha * _pressure * _normals[_qp](1);
    else if(_component == 2)
      return -_alpha * _pressure * _normals[_qp](2);
    else
      mooseError("Unknown stress component");
}
