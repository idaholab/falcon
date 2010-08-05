#include "HuyakornBC.h"

template<>
InputParameters validParams<HuyakornBC>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

HuyakornBC::HuyakornBC(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
   _grad_p(coupledGradient("pressure")),
   _lamda(getMaterialProperty<Real>("lamda")),
   _km(getMaterialProperty<Real>("thermal_conductivity")),
   _dTbydP_H(getMaterialProperty<Real>("dTbydP_H"))

   
  {}

Real
HuyakornBC::computeQpResidual()
  {
    Real theta;

    theta = _lamda[_qp]-(_km[_qp]*_dTbydP_H[_qp]);
    
    
    return -_test[_i][_qp]*theta*_grad_p[_qp]*_normals[_qp];
  }

