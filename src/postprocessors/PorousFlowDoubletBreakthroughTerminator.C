//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowDoubletBreakthroughTerminator.h"

#include <cmath>

registerMooseObject("FalconApp", PorousFlowDoubletBreakthroughTerminator);

InputParameters
PorousFlowDoubletBreakthroughTerminator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addParam<Real>("temperature_init", 273.15, "Reservoir initial temperature");
  params.addParam<Real>("temperature_tolerance", 1e-2, "Absolute temperature difference (in the units of temperature_init) from temperature_init at which breakthrough is declared");
  params.addClassDescription("Returns 1 once a target postprocessor deviates from the reservoir initial temperature by more than temperature_tolerance, and 0 otherwise (thermal breakthrough detection)");
  return params;
}

PorousFlowDoubletBreakthroughTerminator::PorousFlowDoubletBreakthroughTerminator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _temp_init_value(getParam<Real>("temperature_init")),
    _temperature_tolerance(getParam<Real>("temperature_tolerance")),
    _keep_constant(0)
{
}

void
PorousFlowDoubletBreakthroughTerminator::initialize()
{
}

void
PorousFlowDoubletBreakthroughTerminator::execute()
{
  Real temp_diff = std::abs(_pps_value-_temp_init_value);
  if (temp_diff >= _temperature_tolerance)
    _keep_constant = 1;
  else
    _keep_constant = 0;
}

Real
PorousFlowDoubletBreakthroughTerminator::getValue()  const
{
  return _keep_constant;
}
