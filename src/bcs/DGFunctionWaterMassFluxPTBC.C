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

#include "DGFunctionWaterMassFluxPTBC.h"

template<>
InputParameters validParams<DGFunctionWaterMassFluxPTBC>()
{
  InputParameters params = validParams<DGFunctionDiffusionDirichletBC>();

  return params;
}

DGFunctionWaterMassFluxPTBC::DGFunctionWaterMassFluxPTBC(const std::string & name, 
                                             InputParameters parameters) :
  DGFunctionDiffusionDirichletBC(name, parameters),
   _tau_water(getMaterialProperty<Real>("tau_water"))
{}

Real
DGFunctionWaterMassFluxPTBC::computeQpResidual()
{
  return _tau_water[_qp]*DGFunctionDiffusionDirichletBC::computeQpResidual();
}

Real
DGFunctionWaterMassFluxPTBC::computeQpJacobian()
{
  return _tau_water[_qp]*DGFunctionDiffusionDirichletBC::computeQpJacobian();
}
