//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowSteadyStateTerminator.h"

registerMooseObject("FalconApp", PorousFlowSteadyStateTerminator);

InputParameters
PorousFlowSteadyStateTerminator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addParam<Real>(
  "ss_dection_start_time", 0.0,
  "steady state dection start time");
  params.addParam<Real>(
  "ss_dection_end_time", 0.0,
  "steady state dection cap time");
  params.addParam<Real>(
  "ss_relative_error", 1e-2,
  "Postprocessor relative difference");
  params.addClassDescription("provide true or false on steadystatedetection");
  return params;
}

PorousFlowSteadyStateTerminator::PorousFlowSteadyStateTerminator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_t(getPostprocessorValue("timepostprocessor")),
    _steady_state_start_time(getParam<Real>("ss_dection_start_time")),
    _steady_state_end_time(getParam<Real>("ss_dection_end_time")),
    _steady_state_relative_diff(getParam<Real>("ss_relative_error"))
{
}

void
PorousFlowSteadyStateTerminator::initialize()
{
}

void
PorousFlowSteadyStateTerminator::execute()
{
}

Real
PorousFlowSteadyStateTerminator::getValue()
{
  if ((_pps_value <= _steady_state_relative_diff && _pps_t > _steady_state_start_time)|| _pps_t > _steady_state_end_time )
    return 1;
  else
    return 0;
}
