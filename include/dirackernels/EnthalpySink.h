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
#include "PorousFlowPolyLineSink.h"

class SinglePhaseFluidProperties;

/**
 * Enthalpy (energy) sink layered on a PorousFlowPolyLineSink: the mass outflow computed by the
 * base class is multiplied by the fluid enthalpy at the local pressure and a constant inlet
 * temperature T_in.
 */
class EnthalpySink : public PorousFlowPolyLineSink
{
public:
  static InputParameters validParams();

  EnthalpySink(const InputParameters & parameters);

  virtual Real computeQpBaseOutflow(unsigned current_dirac_ptid) const override;
  virtual void computeQpBaseOutflowJacobian(unsigned jvar,
                                            unsigned current_dirac_ptid,
                                            Real & outflow,
                                            Real & outflowp) const override;

protected:
  /// Pressure
  const VariableValue & _pressure;
  /// Specified inlet temperature
  const Real & _T_in;
  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
  /// Pressure variable number (for the enthalpy dh/dp Jacobian term)
  unsigned int _p_var_num;
};
