#pragma once

#include "GeneralPostprocessor.h"

class TimestepPostprocessor;

/**
 * Selects a timestep size (large before, small after) based on when a target postprocessor
 * first exceeds a tolerance relative to a time postprocessor.
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
  const PostprocessorValue & _pps_time;
  /// constant input
  Real _pps_relative_diff;
  Real _charge_time;
  /// Timestep size to use before the trigger fires (s)
  Real _dt_before_trigger;
  /// Timestep size to use after the trigger fires (s)
  Real _dt_after_trigger;

};
