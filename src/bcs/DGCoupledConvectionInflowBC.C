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

/**
 * Authors: Yidong Xia (Yidong.Xia@inl.gov)
 * Created: 09/24/2014
 */

#include "DGCoupledConvectionInflowBC.h"

template<>
InputParameters validParams<DGCoupledConvectionInflowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<Real>("value", "Variable given at boundary");
  params.addRequiredCoupledVar("coupled", "The gradient of this variable will be used as the velocity vector");
  return params;
}

DGCoupledConvectionInflowBC::DGCoupledConvectionInflowBC(const std::string & name,
                               InputParameters parameters) :
  IntegratedBC(name, parameters),
  _value(getParam<Real>("value")),
  _grad_coupled(coupledGradient("coupled"))
{}

Real
DGCoupledConvectionInflowBC::computeQpResidual()
{
  return  -_grad_coupled[_qp] * _normals[_qp] * _value * _test[_i][_qp];
}

Real
DGCoupledConvectionInflowBC::computeQpJacobian()
{
  //No Jacobian
  return 0.0;
}
