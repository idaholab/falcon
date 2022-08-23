//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowTemperatureDropTerminator.h"

registerMooseObject("FalconApp", PorousFlowTemperatureDropTerminator);

InputParameters
PorousFlowTemperatureDropTerminator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("enthalpypostprocessor", "The name of the enthalpy postprocessor");
  params.addRequiredParam<PostprocessorName>("masspostprocessor", "The name of the mass postprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addParam<Real>("T_inj", 0.0, "Injection fluid temperature");
  params.addParam<Real>("T_init", 0.0, "reservior intial temperature");
  params.addParam<Real>("P_drop", 1e-2, "Percent drop");
  // params.addParam<Real>("Cap_time", 0.0, "Dection cap time");
  params.addClassDescription("provide true or false on PorousFlowTemperatureDropTerminator");
  return params;
}

PorousFlowTemperatureDropTerminator::PorousFlowTemperatureDropTerminator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value_J(getPostprocessorValue("enthalpypostprocessor")),
    _pps_value_kg(getPostprocessorValue("masspostprocessor")),
    _pps_t(getPostprocessorValue("timepostprocessor")),
    _temperature_inj(getParam<Real>("T_inj")),
    _temperature_init(getParam<Real>("T_init")),
    // _temperature_detection_cap_time(getParam<Real>("Cap_time")),
    _percentile_drop(getParam<Real>("P_drop"))
{
}

void
PorousFlowTemperatureDropTerminator::initialize()
{
}

void
PorousFlowTemperatureDropTerminator::execute()
{
}

Real
PorousFlowTemperatureDropTerminator::getValue()
{
  if (((_pps_value_J/_pps_value_kg/4186 + 273.15)-_temperature_init)/(_temperature_inj-_temperature_init) < _percentile_drop/100)
    return 1;
  else if (_pps_t >= 315360000)
    return 1;
  else
    return 0;
}
