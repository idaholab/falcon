/*
Material Description of Slip Weakening Friction v3
Generalize the computation of sticking traction using consistent displacement jump and nodal reaction forces
*/

#pragma once

#include "CZMComputeLocalTractionTotalBase.h"

class SlipWeakeningFriction2dv3 : public CZMComputeLocalTractionTotalBase
{
public:
  static InputParameters validParams();
  SlipWeakeningFriction2dv3(const InputParameters & parameters);

protected:
  /// method computing the total traction and its derivatives
  void computeInterfaceTractionAndDerivatives() override;

  Real _T2_o;
  Real _mu_d;
  Real _Dc;

  const VariableValue & _nodal_area;
  const VariableValue & _nodal_area_neighbor;

  const MaterialProperty<Real> & _density;

  const MaterialProperty<RankTwoTensor> & _rot;

  const VariableValue & _disp_slipweakening_x;
  const VariableValue & _disp_slipweakening_neighbor_x;
  const VariableValue & _disp_slipweakening_y;
  const VariableValue & _disp_slipweakening_neighbor_y;

  const VariableValue & _reaction_slipweakening_x;
  const VariableValue & _reaction_slipweakening_neighbor_x;
  const VariableValue & _reaction_slipweakening_y;
  const VariableValue & _reaction_slipweakening_neighbor_y;

  const VariableValue & _disp_slipweakening_x_old;
  const VariableValue & _disp_slipweakening_neighbor_x_old;
  const VariableValue & _disp_slipweakening_y_old;
  const VariableValue & _disp_slipweakening_neighbor_y_old;

};
