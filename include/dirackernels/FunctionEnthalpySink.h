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
 * Enthalpy (energy) sink layered on a PorousFlowPolyLineSink: the mass outflow computed by the
 * base class is multiplied by the fluid enthalpy at the local pressure and an inlet temperature
 * given by a function of time and position.
 */
class FunctionEnthalpySink : public PorousFlowPolyLineSink
{
public:
  static InputParameters validParams();

  FunctionEnthalpySink(const InputParameters & parameters);

  virtual Real computeQpBaseOutflow(unsigned current_dirac_ptid) const override;
  virtual void computeQpBaseOutflowJacobian(unsigned jvar,
                                            unsigned current_dirac_ptid,
                                            Real & outflow,
                                            Real & outflowp) const override;

protected:
  /// Pressure
  const VariableValue & _pressure;
   /// The function being used for evaluation
  const Function & _func;
  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
  /// Pressure variable number (for the enthalpy dh/dp Jacobian term)
  unsigned int _p_var_num;
};
