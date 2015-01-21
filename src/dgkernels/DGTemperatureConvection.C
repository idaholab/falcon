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

#include "Material.h"
#include "DGTemperatureConvection.h"

template<>
InputParameters validParams<DGTemperatureConvection>()
{
  InputParameters params = validParams<DGKernel>();
  params.addParam<Real>("alpha", 1.0, "Scaling Coefficient");
  return params;
}

DGTemperatureConvection::DGTemperatureConvection(const std::string & name,
                                                 InputParameters parameters):
  DGKernel(name, parameters),
  _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
  _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
  _darcy_mass_flux_water_neighbor(getNeighborMaterialProperty<RealGradient>("darcy_mass_flux_water")),
  _alpha(getParam<Real>("alpha"))
{}

Real
DGTemperatureConvection::computeQpResidual(Moose::DGResidualType type)
{
  //Normal darcy mass fluxes on the left and right
  Real fdonl = _darcy_mass_flux_water[_qp]          * _normals[_qp];
  Real fdonr = _darcy_mass_flux_water_neighbor[_qp] * _normals[_qp];

  //Scaled spectral radius
  Real sradi = _alpha*std::max(std::abs(fdonl), std::abs(fdonr));

  //Local Lax-Friedrichlet (LLF) scheme
  Real flux = 0.5 * _specific_heat_water[_qp] *
              ( fdonl*_u[_qp] + fdonr*_u_neighbor[_qp]
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
DGTemperatureConvection::computeQpJacobian(Moose::DGJacobianType type)
{
  //Normal darcy mass fluxes on the left and right
  Real fdonl = _darcy_mass_flux_water[_qp]          * _normals[_qp];
  Real fdonr = _darcy_mass_flux_water_neighbor[_qp] * _normals[_qp];

  //Scaled spectral radius
  Real sradi = _alpha*std::max(std::abs(fdonl), std::abs(fdonr));

  Real r = 0.0;
  switch (type) {
    case Moose::ElementElement:
      r += 0.5 * _specific_heat_water[_qp]   *
                    (  fdonl*_phi[_j][_qp]
                     + sradi*_phi[_j][_qp] ) *
                            _test[_i][_qp];
      break;
    case Moose::ElementNeighbor:
      r += 0.5 * _specific_heat_water[_qp]   *
           (  fdonr*_phi_neighbor[_j][_qp]
            - sradi*_phi_neighbor[_j][_qp] ) *
                            _test[_i][_qp];
      break;
    case Moose::NeighborElement:
      r -= 0.5 * _specific_heat_water[_qp]   *
                    (  fdonl*_phi[_j][_qp]
                     + sradi*_phi[_j][_qp] ) *
                   _test_neighbor[_i][_qp];
      break;
    case Moose::NeighborNeighbor:
      r -= 0.5 * _specific_heat_water[_qp]   *
           (  fdonr*_phi_neighbor[_j][_qp]
            - sradi*_phi_neighbor[_j][_qp] ) *
                   _test_neighbor[_i][_qp];
      break;
  }
  return r;
}
