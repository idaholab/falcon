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

#include "DGCoupledConvectionOutflowBC.h"

template<>
InputParameters validParams<DGCoupledConvectionOutflowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredCoupledVar("coupled", "The gradient of this variable will be used as the velocity vector");
  return params;
}

DGCoupledConvectionOutflowBC::DGCoupledConvectionOutflowBC(const std::string & name, 
                               InputParameters parameters) :
  IntegratedBC(name, parameters),
  _grad_coupled(coupledGradient("coupled"))
{}

Real
DGCoupledConvectionOutflowBC::computeQpResidual()
{
  return  -_grad_coupled[_qp] * _normals[_qp] * _u[_qp] * _test[_i][_qp];
}

Real
DGCoupledConvectionOutflowBC::computeQpJacobian()
{
  return  -_grad_coupled[_qp] * _normals[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}
