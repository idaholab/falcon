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

//! Author:  Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 08/22/2014

#include "DGConvectionInflowBC.h"

template<>
InputParameters validParams<DGConvectionInflowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<Real>("value", "Variable given at inflow");
  params.addRequiredParam<RealVectorValue>("velocity", "Velocity Vector");

  return params;
}

DGConvectionInflowBC::DGConvectionInflowBC(const std::string & name, 
                               InputParameters parameters) :
  IntegratedBC(name, parameters),
  _value(getParam<Real>("value")),
  _velocity(getParam<RealVectorValue>("velocity"))
{}

Real
DGConvectionInflowBC::computeQpResidual()
{
  return  _velocity * _normals[_qp] * _value * _test[_i][_qp];
}

Real
DGConvectionInflowBC::computeQpJacobian()
{
  //No Jacobian
  return 0.0;
}
