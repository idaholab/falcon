//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "DiracKernel.h"
#include "PorousFlowSumQuantity.h"
class Function;
/**
 * Point source (or sink) that adds (removes) fluid at a mass flux rate specified by a postprocessor
 */
class PointSourceSinkFromFunction : public DiracKernel
{
public:
  static InputParameters validParams();

  PointSourceSinkFromFunction(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;
  // virtual Real computeQpJacobian() override;
  // virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

protected:
  /// The function being used for evaluation mass flux
  const Function & _func;
  /// The location of the point source (sink)
  const Point _p;
  /**
   * This is used to hold the total fluid flowing into the line sink for each time step.
   * Hence, it is positive for production wells where fluid is flowing
   * from porespace into the line sink (and hence removed from the model)
   */
  PorousFlowSumQuantity & _total_outflow_mass;
};
