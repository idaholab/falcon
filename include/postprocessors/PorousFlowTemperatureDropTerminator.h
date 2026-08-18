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

class PorousFlowTemperatureDropTerminator;

/**
 * Returns true (1) once the produced fluid temperature drops by a specified percentage
 * relative to the injection and initial temperatures, or a maximum time is exceeded.
 */
class PorousFlowTemperatureDropTerminator : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowTemperatureDropTerminator(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() const override;

protected:

  const PostprocessorValue & _pps_value_J;

  const PostprocessorValue & _pps_value_kg;
    /// current time value
  const PostprocessorValue & _pps_t;

  Real _temperature_inj;
  Real _temperature_init;
  Real _percentile_drop;
  /// Maximum simulation time (s) after which the terminator fires unconditionally
  Real _max_time;

};
