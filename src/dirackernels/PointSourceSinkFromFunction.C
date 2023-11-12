//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PointSourceSinkFromFunction.h"
#include "Function.h"

registerMooseObject("FalconApp", PointSourceSinkFromFunction);

InputParameters
PointSourceSinkFromFunction::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<FunctionName>("mass_flux_function", "The function holding the mass flux at this point in kg/s (positive is flux in, "
      "negative is flux out)");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source (sink)");
  params.addRequiredParam<UserObjectName>(
      "SumQuantityUO",
      "User Object of type=PorousFlowSumQuantity in which to place the total "
      "outflow from the line sink for each time step.");
  params.addClassDescription("This inject or extract fluid from a point via a function");
  return params;
}

PointSourceSinkFromFunction::PointSourceSinkFromFunction(const InputParameters & parameters)
  : DiracKernel(parameters),
    _func(getFunction("mass_flux_function")),
    _p(getParam<Point>("point")),
    _total_outflow_mass(const_cast<PorousFlowSumQuantity &>(getUserObject<PorousFlowSumQuantity>("SumQuantityUO")))
{
  // zero the outflow mass
  _total_outflow_mass.zero();
}

void
PointSourceSinkFromFunction::addPoints()
{
  _total_outflow_mass.zero();
  addPoint(_p, 0);
}

Real
PointSourceSinkFromFunction::computeQpResidual()
{
  Real _mass_flux = _func.value(_t, _q_point[_qp]);
  _total_outflow_mass.add(_mass_flux * _dt) ;
  // Negative sign to make a positive mass_flux in the input file a source
  return _test[_i][_qp] * _mass_flux;
}
