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

