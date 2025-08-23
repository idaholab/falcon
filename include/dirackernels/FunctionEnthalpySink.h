//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "PorousFlowPolyLineSink.h"

class SinglePhaseFluidProperties;
class Function;
/**
 * Point source (or sink) that adds (removes) fluid at a mass flux rate specified by a postprocessor
 */
class FunctionEnthalpySink : public PorousFlowPolyLineSink
{
public:
  static InputParameters validParams();

  FunctionEnthalpySink(const InputParameters & parameters);

  virtual Real computeQpBaseOutflow(unsigned current_dirac_ptid) const override;

protected:
  /// Pressure
  const VariableValue & _pressure;
   /// The function being used for evaluation
  const Function & _func;
  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
};
