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

#include "DGFunctionMaterialDiffusionBC.h"

template<>
InputParameters validParams<DGFunctionMaterialDiffusionBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<std::string>("prop_name", "Property name");
  params.addRequiredParam<FunctionName>("function", "The function for boundary value.");
  params.addParam<Real>("epsilon", 1.0, "Penalty parameter in IP-DG");
  params.addParam<Real>("sigma", 2.0, "Stability parameter in IP-DG");

  return params;
}

DGFunctionMaterialDiffusionBC::DGFunctionMaterialDiffusionBC(const std::string & name,
                                                     InputParameters parameters) :
  IntegratedBC(name, parameters),
  _prop_name(getParam<std::string>("prop_name")),
  _diff(getMaterialProperty<Real>(_prop_name)),
  _func(getFunction("function")),
  _epsilon(getParam<Real>("epsilon")),
  _sigma(getParam<Real>("sigma"))
{}

Real
DGFunctionMaterialDiffusionBC::computeQpResidual()
{
  const unsigned int elem_b_order = static_cast<unsigned int> (_var.getOrder());
  const double h_elem = _current_elem->volume()/_current_side_elem->volume() * 1./std::pow(elem_b_order, 2.);

  Real fn = _func.value(_t, _q_point[_qp]);

  return _diff[_qp] *
         ( - _grad_u[_qp] * _normals[_qp] * _test[_i][_qp]
           + _epsilon * (_u[_qp] - fn) * _grad_test[_i][_qp] * _normals[_qp]
           + _sigma/h_elem * (_u[_qp] - fn) * _test[_i][_qp]
         );
}

Real
DGFunctionMaterialDiffusionBC::computeQpJacobian()
{
  const unsigned int elem_b_order = static_cast<unsigned int> (_var.getOrder());
  const double h_elem = _current_elem->volume()/_current_side_elem->volume() * 1./std::pow(elem_b_order, 2.);

  return _diff[_qp] *
         ( - _grad_phi[_j][_qp] * _normals[_qp] * _test[_i][_qp]
           + _epsilon * _phi[_j][_qp] * _grad_test[_i][_qp] * _normals[_qp]
           + _sigma/h_elem * _phi[_j][_qp] * _test[_i][_qp]
         );
}
