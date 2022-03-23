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
class PorousFlowReporterPointEnthalpySourcePP : public DiracKernel, public ReporterInterface
{
public:
  static InputParameters validParams();

  PorousFlowReporterPointEnthalpySourcePP(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

protected:
  /// The constant mass flux (kg/s)
  const PostprocessorValue & _mass_flux;
  /// Pressure
  const VariableValue & _pressure;
  /// Specified inlet temperature (K)
  const Real & _T_in;
  /// Fluid properties UserObject
  const SinglePhaseFluidProperties & _fp;
  /// x coordinate
  const std::vector<Real> & _x_coord;
  /// y coordinate
  const std::vector<Real> & _y_coord;
  ///z coordinate
  const std::vector<Real> & _z_coord;
  /// Presure variable number
  unsigned int _p_var_num;
};
