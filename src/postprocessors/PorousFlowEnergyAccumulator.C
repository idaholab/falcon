//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowEnergyAccumulator.h"

registerMooseObject("FalconApp", PorousFlowEnergyAccumulator);

InputParameters
PorousFlowEnergyAccumulator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("hotwellenergy", "The name of the enthalpy postprocessor at hot well");
  params.addRequiredParam<PostprocessorName>("coldwellenergy", "The name of the enthalpy postprocessor at cold well");
  params.addRequiredParam<PostprocessorName>("ProductionIndicator", "The name of the postprocessor for production indication");
  params.addClassDescription("accumulate the produced thermal enegy");
  return params;
}

PorousFlowEnergyAccumulator::PorousFlowEnergyAccumulator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_hot(getPostprocessorValue("hotwellenergy")),
    _pps_cold(getPostprocessorValue("coldwellenergy")),
    _pps_pro(getPostprocessorValue("ProductionIndicator")),
    _accumulator(0)
{
}

void
PorousFlowEnergyAccumulator::initialize()
{
}

void
PorousFlowEnergyAccumulator::execute()
{
  if(_pps_pro == 1 ){
    _accumulator += (_pps_hot + _pps_cold);
  }
}

Real
PorousFlowEnergyAccumulator::getValue()
{
  return abs(_accumulator);
}
