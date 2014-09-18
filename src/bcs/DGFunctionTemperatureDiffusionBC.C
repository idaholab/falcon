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

//! Authors: Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 09/16/2014

#include "DGFunctionTemperatureDiffusionBC.h"

template<>
InputParameters validParams<DGFunctionTemperatureDiffusionBC>()
{
  InputParameters params = validParams<DGFunctionDiffusionDirichletBC>();

  return params;
}

DGFunctionTemperatureDiffusionBC::
DGFunctionTemperatureDiffusionBC(const std::string & name, 
                               InputParameters parameters) :
  DGFunctionDiffusionDirichletBC(name, parameters),
  _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
DGFunctionTemperatureDiffusionBC::computeQpResidual()
{
  return _thermal_conductivity[_qp]*DGFunctionDiffusionDirichletBC::computeQpResidual();
}

Real
DGFunctionTemperatureDiffusionBC::computeQpJacobian()
{
  return _thermal_conductivity[_qp]*DGFunctionDiffusionDirichletBC::computeQpJacobian();
}
