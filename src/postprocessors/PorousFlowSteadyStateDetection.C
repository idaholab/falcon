//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowSteadyStateDetection.h"

#include <cmath>

namespace
{
/**
 * Value reported when the relative rate of change cannot be computed yet: the very first
 * evaluation, or a zero appearing in a denominator (dt, dt_old, or value_old).
 *
 * This must read as "definitely NOT steady" rather than "perfectly steady". The companion
 * PorousFlowSteadyStateTerminator tests this value against ss_relative_error (default 1e-2), so
 * returning 0 here would make the documented Detection+Terminator pairing hard-stop a run on its
 * very first timestep and report it as an instantly-converged steady state. 1.0e30 matches the
 * sentinel already used for ss_detection_end_time and accumulator_end_time.
 */
const Real not_computable = 1.0e30;
}

registerMooseObject("FalconApp", PorousFlowSteadyStateDetection);

InputParameters
PorousFlowSteadyStateDetection::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addRequiredParam<PostprocessorName>("targetpostprocessor", "The name of the targetpostprocessor");
  params.addRequiredParam<PostprocessorName>("dt_postprocessor", "The name of the timestep-size postprocessor");
  params.addClassDescription("Computes the rate of relative change in a post-processor value "
                             "over a timestep");
  return params;
}

PorousFlowSteadyStateDetection::PorousFlowSteadyStateDetection(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _pps_value(getPostprocessorValue("targetpostprocessor")),
    _pps_value_old(getPostprocessorValueOld("targetpostprocessor")),
    _pps_dt(getPostprocessorValue("dt_postprocessor")),
    _pps_dt_old(getPostprocessorValueOld("dt_postprocessor"))
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
PorousFlowSteadyStateDetection::getValue()  const
{
  // "Cannot compute yet" must not be mistaken for convergence -- see not_computable.
  if (_t_step == 0 || _pps_dt == 0.0 || _pps_dt_old == 0.0 || _pps_value_old == 0.0)
    return not_computable;

  const Real rate_now = _pps_value / _pps_dt;
  const Real rate_old = _pps_value_old / _pps_dt_old;
  return std::fabs((rate_now - rate_old) / rate_old);
}
