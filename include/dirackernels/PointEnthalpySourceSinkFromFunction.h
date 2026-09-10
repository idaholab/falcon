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
 * Point source/sink that adds or removes heat energy at a variable mass flux rate and a variable
 * temperature, switching between extraction (using the local solution temperature) and injection
 * (using the prescribed temperature_function) based on the sign of mass_flux_function.
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
  /// Which branch the (mass_flux_function, temperature_function) pair selects at the current time
  enum class FlowMode
  {
    Extraction,
    Injection
  };

  /// Classify the current function pair, raising a single diagnostic error if the two are not
  /// coordinated. The predicate is time-dependent (the pair legitimately changes sign between
  /// injection and extraction seasons), so it cannot be hoisted to initialSetup().
  FlowMode currentFlowMode(Real mass_flux, Real T_input) const;

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
