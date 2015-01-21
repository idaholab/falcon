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

#include "DGConvectionOutflowBC.h"

template<>
InputParameters validParams<DGConvectionOutflowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<RealVectorValue>("velocity", "Velocity Vector");

  return params;
}

DGConvectionOutflowBC::DGConvectionOutflowBC(const std::string & name,
                               InputParameters parameters) :
  IntegratedBC(name, parameters),
  _velocity(getParam<RealVectorValue>("velocity"))
{}

Real
DGConvectionOutflowBC::computeQpResidual()
{
  return  _velocity * _normals[_qp] * _u[_qp] * _test[_i][_qp];
}

Real
DGConvectionOutflowBC::computeQpJacobian()
{
  //No Jacobian
  return 0.0;
}
