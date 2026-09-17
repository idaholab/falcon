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

class TimestepPostprocessor;

/**
 * Selects a timestep size (dt_before_trigger before, dt_after_trigger after) based on whether a
 * target postprocessor has yet exceeded a tolerance. The latch is one-way and restartable.
 */
class TimestepPostprocessor : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  TimestepPostprocessor(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() const override;

protected:
  /// current post-processor value
  const PostprocessorValue & _pps_value;
  /// Threshold on _pps_value at which the trigger latches
  Real _pps_relative_diff;
  /// Whether the trigger has fired yet (restartable, so a --recover run does not silently
  /// revert to the pre-trigger timestep size)
  bool & _triggered;
  /// Timestep size to use before the trigger fires (s)
  Real _dt_before_trigger;
  /// Timestep size to use after the trigger fires (s)
  Real _dt_after_trigger;

};
