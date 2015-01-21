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

#include "HuyakornBC.h"

template<>
InputParameters validParams<HuyakornBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

HuyakornBC::HuyakornBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
   _grad_p(coupledGradient("pressure")),
   _lamda(getMaterialProperty<Real>("lamda")),
   _km(getMaterialProperty<Real>("thermal_conductivity")),
   _dT_dP(getMaterialProperty<Real>("dT_dP"))


  {}

Real
HuyakornBC::computeQpResidual()
  {
    Real theta;

    theta = _lamda[_qp]-(_km[_qp]*_dT_dP[_qp]);


    return -_test[_i][_qp]*theta*_grad_p[_qp]*_normals[_qp];
  }

