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

/**
 * Point source (or sink) that adds (removes) fluid at a mass flux rate specified by a postprocessor
 */
class PorousFlowReporterPointSourcePP : public DiracKernel, public ReporterInterface
{
public:
  static InputParameters validParams();

  PorousFlowReporterPointSourcePP(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;

protected:
  /// The constant mass flux (kg/s)
  const PostprocessorValue & _mass_flux;
  /// x coordinate
  const std::vector<Real> & _x_coord;
  /// y coordinate
  const std::vector<Real> & _y_coord;
  ///z coordinate
  const std::vector<Real> & _z_coord;
};
