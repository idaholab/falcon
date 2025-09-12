//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ClosestElemsToLine.h"

class ClosestElemsToLineVarValue : public ClosestElemsToLine
{
public:
  static InputParameters validParams();

  ClosestElemsToLineVarValue(const InputParameters & parameters);

  virtual void initialSetup() override;

  virtual void initialize() override{};
  virtual void execute() override{};
  virtual void finalize() override;

private:
  std::string _var_name;
  std::vector<Real> & _value;
};
