#pragma once

#include "PorousFlowLineGeometry.h"

class SinglePhaseFluidProperties;
class PorousFlowSumQuantity;
class PorousFlowPointFluxQuantity;

namespace libMesh
{
class System;
}

/**
 * Conductive heat exchange between a cased wellbore and the surrounding formation, over the
 * section of a well where casing prevents any fluid mass from crossing between the well and the
 * formation (unlike PorousFlowPeacemanBorehole, whose use_enthalpy=true heat exchange is purely
 * advective: heat flux = mass flux * enthalpy, and is therefore exactly zero wherever the mass
 * flux is zero).
 *
 * The produced fluid keeps flowing up the well through the cased section even though no more
 * mass crosses the casing there, and it exchanges heat with the surrounding formation
 * conductively through the casing wall: dT_well/ds = -(h*P)/(mdot*cp) * (T_well - T_formation),
 * where h is heat_transfer_coefficient (the casing-side resistance only - in-pipe film, steel,
 * cement - NOT an overall wellbore-to-far-field U, since the formation's own conductive
 * resistance is already resolved by the mesh), P is the casing perimeter, mdot is the total
 * produced mass rate, and cp is the fluid's specific heat.
 *
 * The wellbore fluid temperature profile (T_well) is not a solved nonlinear variable: it is
 * recomputed once per residual/Jacobian evaluation (residualSetup()/jacobianSetup(), mirroring
 * PorousFlowPeacemanBorehole::computeWellborePressures()) from the closed-form solution of this
 * ODE using a single length-weighted average formation temperature over the whole cased
 * section, using the mass-flux-weighted mixing temperature of everything produced across the
 * open (perforated) section as the entry condition.
 *
 * Every formation/mixing temperature this computation samples is read from the *old* (previous
 * time step's converged) solution, via SystemBase::solutionOld() - not the current nonlinear
 * iterate - making T_well a true constant throughout a given time step's Newton solve, so
 * computeQpJacobian()'s local-only derivative is exact for that solve rather than an
 * approximation of an otherwise undifferentiated dependency. Sampling the current iterate
 * directly (as PorousFlowPeacemanBorehole does for its own, much more weakly temperature-
 * dependent, unit_weight_temperature) was tried first and destabilized the coupled Newton solve
 * during this example's production ramp-up: unlike that class's hydrostatic pressure integral,
 * T_well's dependence on formation temperature elsewhere along the well can be just as large as
 * the one term the Jacobian captures.
 */
class PorousFlowCasedBoreholeHeatExchange : public PorousFlowLineGeometry
{
public:
  static InputParameters validParams();

  PorousFlowCasedBoreholeHeatExchange(const InputParameters & parameters);

  virtual void initialSetup() override;
  virtual void residualSetup() override;
  virtual void jacobianSetup() override;
  virtual void addPoints() override;

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  /**
   * Nonzero over the cased section of the well (where this kernel is active), zero over the
   * open/perforated section (where PorousFlowPeacemanBorehole already handles mass and heat
   * exchange advectively). The flow rate is multiplied by |character|, as in
   * PorousFlowPeacemanBorehole.
   */
  const Function & _character;

  /// Casing-side heat transfer coefficient (W.m^-2.K^-1), referenced to the casing outer radius
  const Real _h;

  /// Per-point produced mass flux (kg.s^-1) reported by the open interval's mass-extraction
  /// PorousFlowPeacemanBorehole. This kernel's own point set (see PolylineDiracPoints) need not
  /// coincide with the points this is indexed by - the mixing-temperature calculation samples
  /// formation temperature at _mass_flux_x/_mass_flux_y/_mass_flux_z below, not at this kernel's
  /// own _x_coord/_y_coord/_z_coord. Used both to get the total produced mass rate and to
  /// compute the mass-flux-weighted mixing temperature of the fluid entering the cased section.
  const VectorPostprocessorValue & _mass_flux;

  ///@{
  /// Coordinates of the open interval's own mass-flux points, read from the same
  /// PorousFlowPlotPointFluxQuantity's 'x'/'y'/'z' vectors, filled in lockstep with _mass_flux.
  /// The mixing-temperature sum samples the formation temperature at *these* points, not at this
  /// kernel's own points.
  const VectorPostprocessorValue & _mass_flux_x;
  const VectorPostprocessorValue & _mass_flux_y;
  const VectorPostprocessorValue & _mass_flux_z;
  ///@}

  /// Fluid properties used to evaluate the in-well fluid specific heat
  const SinglePhaseFluidProperties & _fp;

  /// Fixed pressure (Pa) at which the in-well fluid specific heat is evaluated
  const Real _reference_pressure;

  /// The libMesh system holding the temperature variable this kernel acts on, used to sample it
  /// at every well point (including ones outside this processor's mesh partition)
  const libMesh::System & _temperature_system;

  /// Variable number of the temperature variable within _temperature_system
  const unsigned int _temperature_var_number;

  /**
   * Wellbore fluid temperature at each well point, indexed by Dirac point ID. Only meaningful
   * over the cased section (nonzero _character); elsewhere holds the local formation
   * temperature, unused by computeQpResidual/computeQpJacobian. Recomputed once per residual
   * evaluation and once per Jacobian evaluation from the *old* (previous time step's converged)
   * solution, so it is a true constant throughout a given time step's Newton solve.
   */
  std::vector<Real> _t_well;

  /// (Re)computes _t_well from the old-solution formation temperature and produced mass flux
  /// profile
  void computeWellboreTemperatures();

  /// Records the total conductive heat exchanged with the formation, per time step (J)
  PorousFlowSumQuantity & _total_outflow;

  /// Records the instantaneous conductive heat rate (W) exchanged with the formation at each
  /// well point, for reporting via a PorousFlowPlotPointFluxQuantity VectorPostprocessor
  PorousFlowPointFluxQuantity & _point_fluxes;
};
