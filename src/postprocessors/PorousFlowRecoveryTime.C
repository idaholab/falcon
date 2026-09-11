//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowRecoveryTime.h"

registerMooseObject("FalconApp", PorousFlowRecoveryTime);

InputParameters
PorousFlowRecoveryTime::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("breakthroughtime", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addClassDescription("Computes the elapsed time (in days) since the recorded doublet breakthrough time");
  return params;
}

PorousFlowRecoveryTime::PorousFlowRecoveryTime(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
   _pps_value(getPostprocessorValue("breakthroughtime")),
   _pps_time(getPostprocessorValue("timepostprocessor"))
{
}

void
PorousFlowRecoveryTime::initialize()
{
}

void
PorousFlowRecoveryTime::execute()
{
}

Real
PorousFlowRecoveryTime::getValue()  const
{
  // PorousFlowDoubletBreakthroughTime reports 0 until it latches. Taken literally that would make
  // this object report the entire elapsed simulation time as "recovery time" before breakthrough
  // has happened at all. A latched breakthrough time is strictly positive, so treat a non-positive
  // value as "not recorded yet".
  if (_pps_value <= 0.0)
    return 0.0;

  return (_pps_time/3600/24 - _pps_value);
}
