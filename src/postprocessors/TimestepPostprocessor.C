//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TimestepPostprocessor.h"

registerMooseObject("FalconApp", TimestepPostprocessor);

InputParameters
TimestepPostprocessor::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addParam<Real>("enthalpy_relative_tolerance", 1e-2, "Postprocessor relative tolerance");
  params.addParam<Real>("dt_before_trigger", 2592000, "The timestep size to use before the trigger fires (Default is 2592000 s, i.e. 1 month)");
  params.addParam<Real>("dt_after_trigger", 10800, "The timestep size to use after the trigger fires (Default is 10800 s, i.e. 6 hours)");
  params.addClassDescription("Selects the simulation timestep size based on whether a target postprocessor has exceeded a tolerance");
  return params;
}

TimestepPostprocessor::TimestepPostprocessor(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_time(getPostprocessorValue("timepostprocessor")),
    _pps_relative_diff(getParam<Real>("enthalpy_relative_tolerance")),
    _charge_time(declareRestartableData<Real>("charge_time", 0)),
    _triggered(declareRestartableData<bool>("triggered", false)),
    _dt_before_trigger(getParam<Real>("dt_before_trigger")),
    _dt_after_trigger(getParam<Real>("dt_after_trigger"))
{
}

void
TimestepPostprocessor::initialize()
{
}

void
TimestepPostprocessor::execute()
{
  if (!_triggered)
  {
    if ( _pps_value >= _pps_relative_diff )
    {
      _charge_time = _pps_time;
      _triggered = true;
    }
  }
}

Real
TimestepPostprocessor::getValue() const
{
  if (!_triggered)
    return _dt_before_trigger; /// 1 month by default
  else
    return _dt_after_trigger; /// 6 hours by default
}
