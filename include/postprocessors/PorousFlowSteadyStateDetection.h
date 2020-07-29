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

class PorousFlowSteadyStateDetection;

template <>
InputParameters validParams<PorousFlowSteadyStateDetection>();

/**
 * Computes the relative change rate in a post-processor value.
 */
class PorousFlowSteadyStateDetection : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowSteadyStateDetection(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() override;

protected:
  /// current post-processor value
  const PostprocessorValue & _pps_value;

  /// old post-processor value
  const PostprocessorValue & _pps_value_old;

  /// current dt value
  const PostprocessorValue & _pps_dt;

  /// old dt
  const PostprocessorValue & _pps_dt_old;
};

