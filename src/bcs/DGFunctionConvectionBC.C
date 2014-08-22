/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
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

#include <cmath>
#include "libmesh/numeric_vector.h"
#include "Material.h"
#include "Function.h"
#include "DGFunctionConvectionBC.h"

template<>
InputParameters validParams<DGFunctionConvectionBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<FunctionName>("function", "The forcing function");

  return params;
}

DGFunctionConvectionBC::DGFunctionConvectionBC(const std::string & name, 
                                             InputParameters parameters) :
  IntegratedBC(name, parameters),
  _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
  _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
  _func(getFunction("function"))
{}

Real
DGFunctionConvectionBC::computeQpResidual()
{
  //Specified value on the boundary
  Real fn = _func.value(_t, _q_point[_qp]);

  return  _specific_heat_water[_qp] * 
          _darcy_mass_flux_water[_qp] * _normals[_qp] * 
          fn * _test[_i][_qp];
}

Real
DGFunctionConvectionBC::computeQpJacobian()
{
  //No Jacobian
  return 0.0;
}
