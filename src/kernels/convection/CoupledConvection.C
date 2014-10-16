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

#include "CoupledConvection.h"

template<>
InputParameters validParams<CoupledConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("coupled", "The gradient of this variable will be used as the velocity vector");
  return params;
}

CoupledConvection::CoupledConvection(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),
  _grad_coupled(coupledGradient("coupled"))
{}

Real CoupledConvection::computeQpResidual()
{
  return _u[_qp] * _grad_coupled[_qp] * _grad_test[_i][_qp];
}

Real CoupledConvection::computeQpJacobian()
{
  return _phi[_j][_qp] * _grad_coupled[_qp] * _grad_test[_i][_qp];
}
