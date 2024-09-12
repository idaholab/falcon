//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowDoubletBreakthroughTerminator.h"

registerMooseObject("FalconApp", PorousFlowDoubletBreakthroughTerminator);

InputParameters
PorousFlowDoubletBreakthroughTerminator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addParam<Real>("temperature_init", 273.15, "Reservior initial temperature");
  params.addParam<Real>("temperature_tolerance", 1e-2, "Postprocessor relative tolerance");
  params.addClassDescription("provide true or false on doublet breakthrough detection");
  return params;
}

PorousFlowDoubletBreakthroughTerminator::PorousFlowDoubletBreakthroughTerminator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _temp_init_value(getParam<Real>("temperature_init")),
    _pps_relative_diff(getParam<Real>("temperature_tolerance")),
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
}

Real
PorousFlowDoubletBreakthroughTerminator::getValue()
{
  Real temp_diff = abs(_pps_value-_temp_init_value);
  if ( _keep_constant == 0)
    if (temp_diff >= _pps_relative_diff )
    {
      _keep_constant = 1;
      return 1;
      }
    else
      return 0;
  else
    return 1;

  // return temp_diff;

}
