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

class SinglePhaseFluidProperties;

/**
 * Point source that adds heat energy corresponding to adding a fluid with flux rate specified by a
 * postprocessor at given temperature (specified by a postprocessor).
 */
class PorousFlowSquarePulsePointEnthalpySink : public DiracKernel
{
public:
  static InputParameters validParams();

  PorousFlowSquarePulsePointEnthalpySink(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

protected:
  /// The constant mass flux (kg/s)
  const Real _mass_flux;
  /// Pressure
  const VariableValue & _pressure;
  /// Quadpoint temperature
  const MaterialProperty<Real> * const _temperature;

  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
  /// The location of the point source
  const Point _p;
  /// The time at which the point source (sink) starts operating
  const Real _start_time;

  /// The time at which the point source (sink) stops operating
  const Real _end_time;
  /// Presure variable number
  unsigned int _p_var_num;
};
