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

class PorousFlowEnergyAccumulator;

/**
 * Accumulates the produced thermal energy (hot + cold well enthalpy) while production is active.
 */
class PorousFlowEnergyAccumulator : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowEnergyAccumulator(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() const override;

protected:
  /// current post-processor value
  const PostprocessorValue & _pps_hot;
  const PostprocessorValue & _pps_cold;
  const PostprocessorValue & _pps_pro;

  Real _accumulator;

};
