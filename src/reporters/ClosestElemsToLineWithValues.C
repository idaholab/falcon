//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ClosestElemsToLineWithValues.h"

registerMooseObject("FalconApp", ClosestElemsToLineWithValues);

InputParameters
ClosestElemsToLineWithValues::validParams()
{
  InputParameters params = ClosestElemsToLine::validParams();
  params.addClassDescription("Creates a reporter named value with a value for each element "
                             "returned by ClosestElemsToLine Reporter.");
  params.addRequiredParam<Real>(
      "value", "value to append to each elem returned by ClosestElemsToLine Reporter");
  return params;
}

ClosestElemsToLineWithValues::ClosestElemsToLineWithValues(const InputParameters & parameters)
  : ClosestElemsToLine(parameters),
    _value(declareValueByName<std::vector<Real>>("value", REPORTER_MODE_REPLICATED))
{
}

void
ClosestElemsToLineWithValues::initialSetup()
{
  ClosestElemsToLine::initialSetup();
  Real value = getParam<Real>("value");
  for (size_t i = 0; i < _eid.size(); ++i)
  {
    _value.push_back(value);
  }
}
