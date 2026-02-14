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
class SinglePhaseFluidProperties;
/**
 * Point source (or sink) that adds (removes) fluid at a constant mass flux rate for times
 * between the specified start and end times. If no start and end times are specified,
 * the source (sink) starts at the start of the simulation and continues to act indefinitely
 */
class PointEnthalpySourceSinkFromFunction : public DiracKernel
{
public:
  static InputParameters validParams();

  PointEnthalpySourceSinkFromFunction(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

protected:
  /**
   * This is used to hold the total fluid flowing into the line sink for each time step.
   * Hence, it is positive for production wells where fluid is flowing
   * from porespace into the line sink (and hence removed from the model)
   */
  PorousFlowSumQuantity & _total_outflow_enthalpy;
  /// The function being used for mass flux (kg/s) function
  const Function & _func_mass_flux;
  /// The function being used for temperature (K) function
  const Function & _func_temperature;
  /// Pressure
  const VariableValue & _pressure;
  /// Quadpoint temperature
  const MaterialProperty<Real> * const _temperature;
  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
  /// The location of the point source (sink)
  const Point _p;
  /// Presure variable number
  unsigned int _p_var_num;

};
