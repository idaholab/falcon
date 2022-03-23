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

class ClosestNodeData : public ClosestNode
{
public:
  static InputParameters validParams();

  ClosestNodeData(const InputParameters & parameters);

  virtual void execute() override;

protected:
  std::string _var_name;
  std::vector<Real> & _var;
};
