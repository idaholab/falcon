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
//! Created: 08/25/2014

#include "CoefDiffusion.h"

template<>
InputParameters validParams<CoefDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.set<Real>("coef")=0.0;
  return params;
}

CoefDiffusion::CoefDiffusion(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _coef(getParam<Real>("coef"))
{}

Real
CoefDiffusion::computeQpResidual()
{
  return _coef*_grad_test[_i][_qp]*_grad_u[_qp];
}

Real
CoefDiffusion::computeQpJacobian()
{
  return _coef*_grad_test[_i][_qp]*_grad_phi[_j][_qp];
}
