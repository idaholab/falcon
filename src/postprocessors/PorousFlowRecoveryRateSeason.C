//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowRecoveryRateSeason.h"

registerMooseObject("FalconApp", PorousFlowRecoveryRateSeason);

InputParameters
PorousFlowRecoveryRateSeason::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("hotwellenergy", "The name of the enthalpy postprocessor at hot well");
  params.addRequiredParam<PostprocessorName>("coldwellenergy", "The name of the enthalpy postprocessor at cold well");
  params.addRequiredParam<PostprocessorName>("InjectionIndicator", "The name of the postprocessor for injeciton indication");
  params.addRequiredParam<PostprocessorName>("ProductionIndicator", "The name of the postprocessor for production indication");
/*  params.addParam<Real>("accumulator_start_time", 0.0, "accumulator start time");
  params.addParam<Real>("accumulator_end_time", 0.0, "accumulator cap time");
  */
  params.addClassDescription("Calculate the recovery rate for the doublet system");
  return params;
}

PorousFlowRecoveryRateSeason::PorousFlowRecoveryRateSeason(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_hot(getPostprocessorValue("hotwellenergy")),
    _pps_cold(getPostprocessorValue("coldwellenergy")),
    _pps_inj(getPostprocessorValue("InjectionIndicator")),
    _pps_pro(getPostprocessorValue("ProductionIndicator")),
    _accumulator_inj(0),
    _accumulator_ext(0)
{
}

void
PorousFlowRecoveryRateSeason::initialize()
{
}

void
PorousFlowRecoveryRateSeason::execute()
{
  if (_pps_inj == 1){
    _accumulator_inj += (_pps_hot + _pps_cold);
  }else if(_pps_pro == 1 ){
    _accumulator_ext += (_pps_hot + _pps_cold);
  }
    
}

Real
PorousFlowRecoveryRateSeason::getValue()
{
  if (_accumulator_ext == 0)
    return 0;
  else
    return abs(_accumulator_ext)/abs(_accumulator_inj)*100;
}
