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
 * Point sink that removes heat energy corresponding to extraction of a fluid at a mass flux rate
 * specified by a postprocessor, evaluated at the LOCAL solution temperature (the
 * PorousFlow_temperature_qp material property) rather than a fixed or prescribed value.
 */
class PointEnthalpySink : public DiracKernel
{
public:
  static InputParameters validParams();

  PointEnthalpySink(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

protected:
  /// The constant mass flux (kg/s)
  const PostprocessorValue & _mass_flux;
  /// Pressure
  const VariableValue & _pressure;
  /// Quadpoint temperature
  const MaterialProperty<Real> * const _temperature;

  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
  /// The location of the point source
  const Point _p;
  /// Presure variable number
  unsigned int _p_var_num;
};
