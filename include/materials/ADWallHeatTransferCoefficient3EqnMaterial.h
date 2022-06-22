#pragma once

#include "Material.h"
#include "FlowChannelBase.h"

class SinglePhaseFluidProperties;

/**
 * Computes liquid wall heat transfer coefficient for pipe geometry using TRACE closures
 */
class ADWallHeatTransferCoefficient3EqnMaterial : public Material
{
public:
  ADWallHeatTransferCoefficient3EqnMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  /// Wall heat transfer coefficient
  ADMaterialProperty<Real> & _Hw;
  /// Fluid density
  const ADMaterialProperty<Real> & _rho;
  /// Fluid velocity
  const ADMaterialProperty<Real> & _vel;
  /// Hydraulic diameter
  const ADMaterialProperty<Real> & _D_h;
  /// Specific volume
  const ADMaterialProperty<Real> & _v;
  /// Specific internal energy
  const ADMaterialProperty<Real> & _e;
  /// Fluid temperature
  const ADMaterialProperty<Real> & _T;
  /// Wall temperature
  const ADMaterialProperty<Real> & _T_wall;
  /// Pressure of the fluid
  const ADMaterialProperty<Real> & _p;
  /// Reynolds number
  const ADMaterialProperty<Real> & _Re;
  /// Prandtl number
  const ADMaterialProperty<Real> & _Pr;
  /// Thermal conductivity of the fluid
  const ADMaterialProperty<Real> & _k;
  /// Dynamic viscosity of the fluid
  const ADMaterialProperty<Real> & _mu;
  /// Gravitational acceleration magnitude
  const Real & _gravity_magnitude;
  /// Fluid properties
  const SinglePhaseFluidProperties & _fp;

public:
  static InputParameters validParams();
};
