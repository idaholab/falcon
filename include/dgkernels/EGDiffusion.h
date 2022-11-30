//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

/**
 * EG kernel for diffusion
 */

#include "DGKernel.h"

class EGDiffusion : public DGKernel
{
public:
  static InputParameters validParams();

  EGDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual(Moose::DGResidualType type) override;
  virtual Real computeQpJacobian(Moose::DGJacobianType type) override;
  virtual Real computeQpOffDiagJacobian(Moose::DGJacobianType type, unsigned int jvar) override;

  Real _epsilon;
  const MaterialProperty<Real> & _sigma;
  const MaterialProperty<Real> & _sigma_neighbor;
  const MaterialProperty<Real> & _diff;
  const MaterialProperty<Real> & _diff_neighbor;
  MooseVariable & _v_var;
  const VariableValue & _v;
  const VariableValue & _v_neighbor;
  const VariableGradient & _grad_v;
  const VariableGradient & _grad_v_neighbor;
  unsigned int _v_id;
};
