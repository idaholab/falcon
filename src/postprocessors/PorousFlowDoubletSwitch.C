//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowDoubletSwitch.h"
#include <math.h>

registerMooseObject("FalconApp", PorousFlowDoubletSwitch);

InputParameters
PorousFlowDoubletSwitch::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addParam<Real>("temperature_init", 273.15, "Reservior initial temperature");
  params.addParam<Real>("temperature_tolerance", 1e-2, "Postprocessor relative tolerance");
  params.addClassDescription("provide true or false on doublet breakthrough detection");
  return params;
}

PorousFlowDoubletSwitch::PorousFlowDoubletSwitch(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_time(getPostprocessorValue("timepostprocessor")),
    _temp_init_value(getParam<Real>("temperature_init")),
    _pps_relative_diff(getParam<Real>("temperature_tolerance")),
    _charge_time(0)
{
}

void
PorousFlowDoubletSwitch::initialize()
{
}

void
PorousFlowDoubletSwitch::execute()
{
  if (_charge_time == 0)
  {
    if (abs(_pps_value-_temp_init_value) >= _pps_relative_diff )
    {
      _charge_time = _pps_time;
    }
  }
}

Real
PorousFlowDoubletSwitch::getValue()
{
  if (_charge_time == 0)
    return 0;
  else
  {
    Real temp_time;
    Real check;
    temp_time = _pps_time - _charge_time;
    check = temp_time/3600/24 - floor(temp_time/3600/24);
    if ( check < 0.6666667 )
      return 1;
    else
      return 0;
  }
}


