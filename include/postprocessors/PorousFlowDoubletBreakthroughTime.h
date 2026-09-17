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

class PorousFlowDoubletBreakthroughTime;

/**
 * Records the simulation time (in days) at which the doublet breakthrough terminator
 * postprocessor first indicates breakthrough.
 */
class PorousFlowDoubletBreakthroughTime : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowDoubletBreakthroughTime(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() const override;

protected:
  /// current post-processor value
  const PostprocessorValue & _pps_value;
  const PostprocessorValue & _pps_time;
  /// constant input

  /// Recorded breakthrough time (restartable)
  Real & _keep_constant;
  /// Whether breakthrough has been recorded yet (restartable; kept separate from
  /// _keep_constant so a breakthrough detected exactly at t=0 isn't indistinguishable from
  /// "not yet detected")
  bool & _triggered;

};
