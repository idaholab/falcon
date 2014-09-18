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
//! Created: 08/18/2014

#include "DGWaterMassFluxPressure_PT.h"

template<>
InputParameters validParams<DGWaterMassFluxPressure_PT>()
{
  InputParameters params = validParams<DGKernel>();
  params.addParam<Real>("epsilon", 1.0, "epsilon: = -1 SIPG; = 1 NIPG; = 0 IIPG. Default = 1.0");
  params.addParam<Real>("sigma", 2.0, "sigma: stability parameter. Default = 2.0");
  return params;
}

DGWaterMassFluxPressure_PT::DGWaterMassFluxPressure_PT(const std::string & name,
                                                   InputParameters parameters) :
  DGKernel(name, parameters),
  _tau_water(getMaterialProperty<Real>("tau_water")),
  _tau_water_neighbor(getNeighborMaterialProperty<Real>("tau_water")),
  _epsilon(getParam<Real>("epsilon")),
  _sigma(getParam<Real>("sigma"))
{}

Real
DGWaterMassFluxPressure_PT::computeQpResidual(Moose::DGResidualType type)
{
  const unsigned int elem_b_order = static_cast<unsigned int> (_var.getOrder());
  const double h_elem = _current_elem->volume()/_current_side_elem->volume() * 1./std::pow(elem_b_order, 2.);

  Real r = 0;
  switch (type)
  {
  case Moose::Element:
    r = - 0.5 * (_tau_water[_qp]*_grad_u[_qp] + _tau_water_neighbor[_qp]*_grad_u_neighbor[_qp]) * _normals[_qp] * _test[_i][_qp]
        + _epsilon * 0.5 * (_tau_water[_qp]*_u[_qp] - _tau_water_neighbor[_qp]*_u_neighbor[_qp]) * _grad_test[_i][_qp] * _normals[_qp]
        + _sigma / h_elem * (_tau_water[_qp]*_u[_qp] - _tau_water_neighbor[_qp]*_u_neighbor[_qp]) * _test[_i][_qp];
    break;

  case Moose::Neighbor:
    r = + 0.5 * (_tau_water[_qp]*_grad_u[_qp] + _tau_water_neighbor[_qp]*_grad_u_neighbor[_qp]) * _normals[_qp] * _test_neighbor[_i][_qp]
        + _epsilon * 0.5 * (_tau_water[_qp]*_u[_qp] - _tau_water_neighbor[_qp]*_u_neighbor[_qp]) * _grad_test_neighbor[_i][_qp] * _normals[_qp]
        - _sigma / h_elem * (_tau_water[_qp]*_u[_qp] - _tau_water_neighbor[_qp]*_u_neighbor[_qp]) * _test_neighbor[_i][_qp];
    break;
  }
  return r;
}

Real
DGWaterMassFluxPressure_PT::computeQpJacobian(Moose::DGJacobianType type)
{
  const unsigned int elem_b_order = static_cast<unsigned int> (_var.getOrder());
  const double h_elem = _current_elem->volume()/_current_side_elem->volume() * 1./std::pow(elem_b_order, 2.);

  Real r = 0;
  switch (type)
  {
  case Moose::ElementElement:
    r = - 0.5 * _tau_water[_qp] * _grad_phi[_j][_qp] * _normals[_qp] * _test[_i][_qp]
        + _epsilon * 0.5 * _tau_water[_qp] * _phi[_j][_qp] * _grad_test[_i][_qp] * _normals[_qp]
        + _sigma / h_elem * _tau_water[_qp] * _phi[_j][_qp] * _test[_i][_qp];
    break;

  case Moose::ElementNeighbor:
    r = - 0.5 * _tau_water_neighbor[_qp] * _grad_test[_j][_qp] * _normals[_qp] * _test_neighbor[_i][_qp]
        - _epsilon * 0.5 * _tau_water_neighbor[_qp] * _phi_neighbor[_j][_qp] * _grad_test[_i][_qp] * _normals[_qp]
        - _sigma / h_elem * _test[_j][_qp] * _tau_water[_qp] * _test_neighbor[_i][_qp];
    break;

  case Moose::NeighborElement:
    r = + 0.5 * _tau_water[_qp] * _grad_phi[_j][_qp] * _normals[_qp] * _test_neighbor[_i][_qp]
        + _epsilon * 0.5 * _tau_water[_qp] * _phi[_j][_qp] * _grad_test_neighbor[_i][_qp] * _normals[_qp]
        - _sigma / h_elem * _tau_water[_qp] * _phi[_j][_qp] * _test_neighbor[_i][_qp];
    break;

  case Moose::NeighborNeighbor:
    r = + 0.5 * _tau_water_neighbor[_qp] * _grad_phi_neighbor[_j][_qp] * _normals[_qp] * _test_neighbor[_i][_qp]
        - _epsilon * 0.5 * _tau_water_neighbor[_qp] * _phi_neighbor[_j][_qp] * _grad_test_neighbor[_i][_qp] * _normals[_qp]
        + _sigma / h_elem * _tau_water_neighbor[_qp] * _phi_neighbor[_j][_qp] * _test_neighbor[_i][_qp];
    break;
  }
  return r;
}
