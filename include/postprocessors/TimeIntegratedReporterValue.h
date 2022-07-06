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

/**
 * Integrate a reporter value over time using trapezoidal rule
 */
class TimeIntegratedReporterValue : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  TimeIntegratedReporterValue(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() override;

protected:
  /// The total value of the variable
  Real _value;

  /// My old value
  Real _value_old;

  /// The current post-processor value
  std::vector<Real> _pps_value;

  /// The old post-processor value
  Real _pps_value_old;

  /// Return negative value of the output
  const bool & _use_negative_value;
};
