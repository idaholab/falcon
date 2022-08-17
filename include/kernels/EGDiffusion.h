#pragma once

#include "DGKernel.h"

// Forward Declarations
class EGDiffusion;

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
