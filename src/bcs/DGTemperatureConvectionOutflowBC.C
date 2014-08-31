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
//! Created: 08/27/2014

#include "Material.h"
#include "DGTemperatureConvectionOutflowBC.h"

template<>
InputParameters validParams<DGTemperatureConvectionOutflowBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  return params;
}

DGTemperatureConvectionOutflowBC::DGTemperatureConvectionOutflowBC(const std::string & name, 
                                             InputParameters parameters) :
  IntegratedBC(name, parameters),
  _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
  _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water"))
{}

Real
DGTemperatureConvectionOutflowBC::computeQpResidual()
{
  return  _specific_heat_water[_qp] * 
          _darcy_mass_flux_water[_qp] * _normals[_qp] * 
          _u[_qp] * _test[_i][_qp];
}

Real
DGTemperatureConvectionOutflowBC::computeQpJacobian()
{
  return  _specific_heat_water[_qp] * 
          _darcy_mass_flux_water[_qp] * _normals[_qp] * 
          _phi[_j][_qp] * _test[_i][_qp];
}
