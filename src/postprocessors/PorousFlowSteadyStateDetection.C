//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowSteadyStateDetection.h"

registerMooseObject("FalconApp", PorousFlowSteadyStateDetection);

InputParameters
PorousFlowSteadyStateDetection::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("timepostprocessor", "The name of the timepostprocessor");
  params.addClassDescription("Computes the rate of relative change in a post-processor value "
                             "over a timestep");
  return params;
}

PorousFlowSteadyStateDetection::PorousFlowSteadyStateDetection(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_value_old(getPostprocessorValueOld("targetpostprocessor")),
    _pps_dt(getPostprocessorValue("timepostprocessor")),
    _pps_dt_old(getPostprocessorValueOld("timepostprocessor"))
{
}

void
PorousFlowSteadyStateDetection::initialize()
{
}

void
PorousFlowSteadyStateDetection::execute()
{
}

Real
PorousFlowSteadyStateDetection::getValue()
{
  // copy initial value in case difference is measured against initial value
  Real change;
  if (_t_step == 0)
    change  = 0;
  else
    change = (_pps_value/_pps_dt - _pps_value_old/_pps_dt_old)/(_pps_value_old/_pps_dt_old);
  return std::fabs(change);
}
