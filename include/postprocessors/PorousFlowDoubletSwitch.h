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

class PorousFlowDoubletSwitch;

///template <>
///InputParameters validParams<PorousFlowSteadyStateDetection>();

/**
 * Computes the relative change rate in a post-processor value.
 */
class PorousFlowDoubletSwitch : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowDoubletSwitch(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() override;

protected:
  /// current post-processor value
  const PostprocessorValue & _pps_value;
  const PostprocessorValue & _pps_time;
  /// constant input
  Real _temp_init_value;
  Real _pps_relative_diff;
  Real _charge_time;

};
