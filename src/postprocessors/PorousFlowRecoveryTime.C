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
  params.addClassDescription("provide true or false on doublet breakthrough detection");
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
PorousFlowRecoveryTime::getValue()
{
  return (_pps_time/3600/24 - _pps_value);
}
