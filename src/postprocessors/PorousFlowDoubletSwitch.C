//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowDoubletSwitch.h"

#include <cmath>

registerMooseObject("FalconApp", PorousFlowDoubletSwitch);

InputParameters
PorousFlowDoubletSwitch::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addParam<Real>("temperature_init", 273.15, "Reservoir initial temperature");
  params.addParam<Real>("temperature_tolerance", 1e-2, "Absolute temperature difference (in the units of temperature_init) from temperature_init at which breakthrough is declared");
  params.addRangeCheckedParam<Real>("duty_cycle_fraction", 0.6666667, "duty_cycle_fraction >= 0 & duty_cycle_fraction <= 1", "Fraction of each daily cycle during which the doublet switch is on");
  params.addClassDescription("Returns 1 (on) or 0 (off) for a doublet well following a daily duty cycle, which starts once a target postprocessor first deviates from the reservoir initial temperature by more than temperature_tolerance");
  return params;
}

PorousFlowDoubletSwitch::PorousFlowDoubletSwitch(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_time(getPostprocessorValue("timepostprocessor")),
    _temp_init_value(getParam<Real>("temperature_init")),
    _temperature_tolerance(getParam<Real>("temperature_tolerance")),
    _charge_time(declareRestartableData<Real>("charge_time", 0)),
    _triggered(declareRestartableData<bool>("triggered", false)),
    _duty_cycle_fraction(getParam<Real>("duty_cycle_fraction"))
{
}

void
PorousFlowDoubletSwitch::initialize()
{
}

void
PorousFlowDoubletSwitch::execute()
{
  if (!_triggered)
  {
    if (std::abs(_pps_value-_temp_init_value) >= _temperature_tolerance )
    {
      _charge_time = _pps_time;
      _triggered = true;
    }
  }
}

Real
PorousFlowDoubletSwitch::getValue()  const
{
  if (!_triggered)
    return 0;
  else
  {
    Real temp_time;
    Real check;
    temp_time = _pps_time - _charge_time;
    check = temp_time/3600/24 - std::floor(temp_time/3600/24);
    if ( check < _duty_cycle_fraction )
      return 1;
    else
      return 0;
  }
}


