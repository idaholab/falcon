//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "timestepPostprocessor.h"

registerMooseObject("FalconApp", timestepPostprocessor);

InputParameters
timestepPostprocessor::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addParam<Real>("enthalpy_relative_tolerance", 1e-2, "Postprocessor relative tolerance");
  params.addClassDescription("provide true or false on doublet breakthrough detection");
  return params;
}

timestepPostprocessor::timestepPostprocessor(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_time(getPostprocessorValue("timepostprocessor")),
    _pps_relative_diff(getParam<Real>("enthalpy_relative_tolerance")),
    _charge_time(0)
{
}

void
timestepPostprocessor::initialize()
{
}

void
timestepPostprocessor::execute()
{
  if (_charge_time == 0)
  {
    if ( _pps_value >= _pps_relative_diff )
    {
      _charge_time = _pps_time;
    }
  }
}

Real
timestepPostprocessor::getValue()
{
  if (_charge_time == 0)
    return 2592000; /// 1 months 3600*24*30
  else
    return 10800; /// 6 hours
}


