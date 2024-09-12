//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowAccumulator.h"

registerMooseObject("FalconApp", PorousFlowAccumulator);

InputParameters
PorousFlowAccumulator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
/*  params.addParam<Real>("accumulator_start_time", 0.0, "accumulator start time");
  params.addParam<Real>("accumulator_end_time", 0.0, "accumulator cap time");
*/
  params.addClassDescription("accumulate the targetted post processor value");
  return params;
}

PorousFlowAccumulator::PorousFlowAccumulator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_t(getPostprocessorValue("timepostprocessor")),
/*    _accumulator_start_time(getParam<Real>("accumulator_start_time")),
    _accumulator_end_time(getParam<Real>("accumulator_end_time")),
*/
    _accumulator(0)
{
}

void
PorousFlowAccumulator::initialize()
{
}

void
PorousFlowAccumulator::execute()
{
///  if (_pps_t > _accumulator_start_time &&  _pps_t < _accumulator_end_time)
    if (_pps_value < 0)
      _accumulator += _pps_value;
}

Real
PorousFlowAccumulator::getValue()
{
  return abs(_accumulator);
}
