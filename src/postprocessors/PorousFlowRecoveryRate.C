//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowRecoveryRate.h"

#include <cmath>

registerMooseObject("FalconApp", PorousFlowRecoveryRate);

InputParameters
PorousFlowRecoveryRate::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("hotwellenergy", "The name of the enthalpy postprocessor at hot well");
  params.addRequiredParam<PostprocessorName>("coldwellenergy", "The name of the enthalpy postprocessor at cold well");
  params.addClassDescription("Calculate the recovery rate for the doublet system");
  return params;
}

PorousFlowRecoveryRate::PorousFlowRecoveryRate(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_hot(getPostprocessorValue("hotwellenergy")),
    _pps_cold(getPostprocessorValue("coldwellenergy")),
    _accumulator_inj(declareRestartableData<Real>("accumulator_inj", 0)),
    _accumulator_ext(declareRestartableData<Real>("accumulator_ext", 0))
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
PorousFlowRecoveryRate::getValue()  const
{
  if (_accumulator_inj == 0)
    return 0;
  else
    return std::abs(_accumulator_ext)/std::abs(_accumulator_inj)*100;
}
