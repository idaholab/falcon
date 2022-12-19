//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowDoubletBreakthroughTime.h"

registerMooseObject("FalconApp", PorousFlowDoubletBreakthroughTime);

InputParameters
PorousFlowDoubletBreakthroughTime::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("breakthroughterminator", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addClassDescription("provide true or false on doublet breakthrough detection");
  return params;
}

PorousFlowDoubletBreakthroughTime::PorousFlowDoubletBreakthroughTime(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("breakthroughterminator")),
    _pps_time(getPostprocessorValue("timepostprocessor")),
    _keep_constant(0)
{
}

void
PorousFlowDoubletBreakthroughTime::initialize()
{
}

void
PorousFlowDoubletBreakthroughTime::execute()
{
  if (_keep_constant == 0 && _pps_value ==1)
    _keep_constant = _pps_time/3600/24;
}

Real
PorousFlowDoubletBreakthroughTime::getValue()
{
  return _keep_constant;
}
