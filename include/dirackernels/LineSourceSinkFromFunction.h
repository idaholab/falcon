//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "PorousFlowLineSink.h"

/**
 * Approximates a line sink by a sequence of Dirac Points
 */
class LineSourceSinkFromFunction : public PorousFlowLineSink
{
public:
  static InputParameters validParams();

  LineSourceSinkFromFunction(const InputParameters & parameters);

protected:

  virtual Real computeQpBaseOutflow(unsigned current_dirac_ptid) const override;
  virtual void computeQpBaseOutflowJacobian(unsigned jvar,
                                            unsigned current_dirac_ptid,
                                            Real & outflow,
                                            Real & outflowp) const override;
//   / The function being used for evaluation mass flux
  const Function & _func;
};
