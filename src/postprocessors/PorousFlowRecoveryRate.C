//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowRecoveryRate.h"

registerMooseObject("FalconApp", PorousFlowRecoveryRate);

InputParameters
PorousFlowRecoveryRate::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("hotwellenergy", "The name of the enthalpy postprocessor at hot well");
  params.addRequiredParam<PostprocessorName>("coldwellenergy", "The name of the enthalpy postprocessor at cold well");
/*  params.addParam<Real>("accumulator_start_time", 0.0, "accumulator start time");
  params.addParam<Real>("accumulator_end_time", 0.0, "accumulator cap time");
  */
  params.addClassDescription("Calculate the recovery rate for the doublet system");
  return params;
}

PorousFlowRecoveryRate::PorousFlowRecoveryRate(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_hot(getPostprocessorValue("hotwellenergy")),
    _pps_cold(getPostprocessorValue("coldwellenergy")),
/*    _accumulator_start_time(getParam<Real>("accumulator_start_time")),
    _accumulator_end_time(getParam<Real>("accumulator_end_time")),
    */
    _accumulator_inj(0),
    _accumulator_ext(0)
{
}

void
PorousFlowRecoveryRate::initialize()
{
}

void
PorousFlowRecoveryRate::execute()
{
  if (_pps_hot <= 0)
    _accumulator_inj += (_pps_hot + _pps_cold);
  else
    _accumulator_ext += (_pps_hot + _pps_cold);
}

Real
PorousFlowRecoveryRate::getValue()
{
  if (_accumulator_ext == 0)
    return 0;
  else
    return abs(_accumulator_ext)/abs(_accumulator_inj)*100;
}
