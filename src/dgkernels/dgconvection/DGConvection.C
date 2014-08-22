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
//! Created: 08/19/2014

#include "DGConvection.h"

template<>
InputParameters validParams<DGConvection>()
{
  InputParameters params = validParams<DGKernel>();
  params.addParam<Real>("alpha", 1.0, "Scaling Coefficient");
  params.addRequiredParam<RealVectorValue>("velocity", "Velocity Vector");
  return params;
}

DGConvection::DGConvection(const std::string & name, InputParameters parameters) :
  DGKernel(name, parameters),
  _alpha(getParam<Real>("alpha")),
  _velocity(getParam<RealVectorValue>("velocity"))
{}

Real
DGConvection::computeQpResidual(Moose::DGResidualType type)
{
  /**
   * Note: this only serves as a simple example to show
   *       how to write DG convection kernels.
   *       In most cases the velocity is coupled,
   *       and you will expect more complex flux schemes
   */

  //Normal velocity on the left and right
  Real vdonl = _velocity*_normals[_qp];
  Real vdonr = _velocity*_normals[_qp];

  //Scaled spectral radius
  Real sradi = _alpha*std::max(std::abs(vdonl), std::abs(vdonr));

  //Local Lax-Friedrichlet (LLF) scheme
  Real flux = 0.5*( vdonl*_u[_qp] + vdonr*_u_neighbor[_qp] 
                    - sradi*(_u_neighbor[_qp]-_u[_qp]) );

  Real r = 0.0;

  switch (type) {
    case Moose::Element:
      r += flux * _test[_i][_qp];
      break;
    case Moose::Neighbor:
      r -= flux * _test_neighbor[_i][_qp];
      break;
  }

  return r;
}

Real
DGConvection::computeQpJacobian(Moose::DGJacobianType type)
{
  /**
   * Note: this only serves as a simple example 
   *       how to write DG convection Jacobian kernels.
   *       In most cases the velocity is coupled,
   *       and you will expect more complex Jacobian matrices
   */

  //Normal velocity on the left and right
  Real vdonl = _velocity*_normals[_qp];
  Real vdonr = _velocity*_normals[_qp];

  //Scaled spectral radius
  Real sradi = _alpha*std::max(std::abs(vdonl), std::abs(vdonr));

  Real fj= 0.0; 
  Real r = 0.0;

  switch (type) {
    case Moose::ElementElement:
      fj = 0.5*( vdonl*_phi[_j][_qp] + sradi*_phi[_j][_qp] );
      r += fj * _test[_i][_qp];
      break;
    case Moose::ElementNeighbor:
      fj = 0.5*( vdonr*_phi_neighbor[_j][_qp] - sradi*_phi_neighbor[_j][_qp] );
      r += fj * _test[_i][_qp];
      break;
    case Moose::NeighborElement:
      fj = 0.5*( vdonl*_phi[_j][_qp] + sradi*_phi[_j][_qp] );
      r -= fj * _test_neighbor[_i][_qp];
      break;
    case Moose::NeighborNeighbor:
      fj = 0.5*( vdonr*_phi_neighbor[_j][_qp] - sradi*_phi_neighbor[_j][_qp] );
      r -= fj * _test_neighbor[_i][_qp];
      break;
  }

  return r;
}

