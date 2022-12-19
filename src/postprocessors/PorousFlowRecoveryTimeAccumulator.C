//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowRecoveryTimeAccumulator.h"

registerMooseObject("FalconApp", PorousFlowRecoveryTimeAccumulator);

InputParameters
PorousFlowRecoveryTimeAccumulator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("dtpostprocessor", "The name of the dtpostprocessor");
  params.addClassDescription("provide functioning time accumulation");
  return params;
}

PorousFlowRecoveryTimeAccumulator::PorousFlowRecoveryTimeAccumulator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
   _pps_iswinter(getPostprocessorValue("targetpostprocessor")),
   _pps_dt(getPostprocessorValue("dtpostprocessor")),
   _accumulator(0)
{
}

void
PorousFlowRecoveryTimeAccumulator::initialize()
{
}

void
PorousFlowRecoveryTimeAccumulator::execute()
{
    if (_pps_iswinter)
      _accumulator += _pps_dt;
}

Real
PorousFlowRecoveryTimeAccumulator::getValue()
{
  return abs(_accumulator/3600/24);
}
