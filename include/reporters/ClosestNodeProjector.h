//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ClosestNode.h"

class ClosestNodeProjector : public ClosestNode
{
public:
  static InputParameters validParams();

  ClosestNodeProjector(const InputParameters & parameters);

  virtual void execute() override {}

protected:
  const std::vector<Real> & _point_value;
  std::vector<Real> & _node_value;
};
