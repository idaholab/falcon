//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GeneralPostprocessor.h"

class PorousFlowAccumulator;

/**
 * Accumulates the value of a target postprocessor over time (optionally gated by a time window).
 */
class PorousFlowAccumulator : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowAccumulator(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() const override;

protected:
  /// current post-processor value
  const PostprocessorValue & _pps_value;

  /// current time value
  const PostprocessorValue & _pps_t;

  /// Time at which accumulation starts
  Real _accumulator_start_time;
  /// Time at which accumulation ends
  Real _accumulator_end_time;
  Real _accumulator;

};

