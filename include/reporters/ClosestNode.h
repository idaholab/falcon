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

class ClosestNode : public GeneralReporter
{
public:
  static InputParameters validParams();

  ClosestNode(const InputParameters & parameters);

  virtual void initialSetup() override;

  virtual void initialize() override {}
  virtual void execute() override {}
  virtual void finalize() override {}

protected:
  const std::vector<Real> & _point_x;
  const std::vector<Real> & _point_y;
  const std::vector<Real> & _point_z;
  const Real _tolerance;

  std::vector<dof_id_type> & _nid;
  std::vector<Real> & _node_x;
  std::vector<Real> & _node_y;
  std::vector<Real> & _node_z;
  std::vector<const Node *> _node_ptrs;
};
