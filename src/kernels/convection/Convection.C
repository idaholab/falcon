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
//! Created: 08/20/2014

#include "Convection.h"

template<>
InputParameters validParams<Convection>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<RealVectorValue>("velocity", "Velocity Vector");
  return params;
}

Convection::Convection(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
   _velocity(getParam<RealVectorValue>("velocity"))
{}

Real Convection::computeQpResidual()
{
  return - _u[_qp]*_velocity*_grad_test[_i][_qp];
}

Real Convection::computeQpJacobian()
{
  return - _phi[_j][_qp]* _velocity*_grad_test[_i][_qp];
}
