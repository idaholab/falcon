//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GeneralReporter.h"

class ClosestNodeProjector : public GeneralReporter
{
public:
  static InputParameters validParams();

  ClosestNodeProjector(const InputParameters & parameters);

  virtual void initialize() override {}
  virtual void execute() override;
  virtual void finalize() override {}

protected:
  std::vector<Real> & _xcoord;
  std::vector<Real> & _ycoord;
  std::vector<Real> & _zcoord;
  const std::vector<Real> & _values;
  const Real _tolerance;
};
