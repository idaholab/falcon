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

#include "DGFunctionTemperatureConvectionInflowBC.h"

template<>
InputParameters validParams<DGFunctionTemperatureConvectionInflowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<FunctionName>("function", "The function for inflow value");

  return params;
}

DGFunctionTemperatureConvectionInflowBC::DGFunctionTemperatureConvectionInflowBC(const std::string & name, 
                                             InputParameters parameters) :
  IntegratedBC(name, parameters),
  _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
  _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
  _func(getFunction("function"))
{}

Real
DGFunctionTemperatureConvectionInflowBC::computeQpResidual()
{
  //Specified value on the boundary
  Real fn = _func.value(_t, _q_point[_qp]);

  return  _specific_heat_water[_qp] * 
          _darcy_mass_flux_water[_qp] * _normals[_qp] * 
          fn * _test[_i][_qp];
}

Real
DGFunctionTemperatureConvectionInflowBC::computeQpJacobian()
{
  //No Jacobian
  return 0.0;
}
