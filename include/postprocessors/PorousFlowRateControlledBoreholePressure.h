#pragma once

#include "GeneralPostprocessor.h"

class Function;

/**
 * Corrects a Peaceman borehole's "bottom_p_or_t" pressure once per timestep, by a secant-method
 * update on the well's own delivered mass rate, so that a rate-controlled (rather than
 * pressure-controlled) well can be modelled without any change to PorousFlowPeacemanBorehole
 * itself.
 *
 * Peaceman's flux at each point is WI_i * mobility_i * (reservoir_pressure_i - wellbore_pressure_i),
 * and wellbore_pressure_i = bottom_p_or_t + (a per-point hydrostatic/thermal offset independent
 * of bottom_p_or_t), so for a momentarily-frozen permeability/mobility state the TOTAL well rate
 * is an affine function of the single scalar bottom_p_or_t: Q(P) = A - B*P, where
 * B = sum_i(WI_i * mobility_i) is the well's conductance. That means a plain secant-method
 * update - using only the well's own delivered rate and its own bottom_p_or_t history, both
 * already-available scalars - finds the pressure that would deliver a target rate, with no need
 * to evaluate WI_i (hence no need for permeability) at all. This matters because WI_i depends on
 * a MaterialProperty (permeability) that is only valid inside the normal per-quadrature-point
 * Dirac-kernel evaluation loop - there is no way to assemble it analytically from a
 * residualSetup()-style calculation done before any point has been visited, the way this
 * example's own PorousFlowCasedBoreholeHeatExchange::computeWellboreTemperatures() does for the
 * wellbore *temperature* profile (which only ever needs to sample a solved field, not a material
 * property).
 *
 * This object runs at EXEC_TIMESTEP_BEGIN only (enforced in the constructor): that keeps the
 * pressure it reports a true constant through the entire following timestep's Newton solve,
 * exactly the same "frozen for the whole timestep" property that makes
 * PorousFlowCasedBoreholeHeatExchange's own old-solution lagging of the wellbore temperature
 * exact rather than approximate.
 *
 * A locally-measured secant slope can under-estimate the true correction needed once conditions
 * (or the target itself) have drifted far enough from where that slope was measured - real
 * mobility is not perfectly constant with pressure/temperature, so the affine assumption above
 * only holds in a neighbourhood of the operating point it was fit at. min_relative_error_for_floor
 * escalates the correction while the relative error remains large, but only when the correction
 * is pushing the same direction as the previous call's - not when alternating sign call to call,
 * which indicates the controller is already oscillating around a target it is close to, and
 * escalating further would only make that oscillation worse. See this parameter's own
 * documentation for the full reasoning.
 */
class PorousFlowRateControlledBoreholePressure : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  PorousFlowRateControlledBoreholePressure(const InputParameters & parameters);

  virtual void initialize() override {}
  virtual void execute() override;
  virtual Real getValue() const override;

protected:
  /// The well's own delivered signed mass rate (kg.s^-1), read as of the start of this timestep -
  /// which, since this executes at EXEC_TIMESTEP_BEGIN (after old-state has already been
  /// advanced), is the previous timestep's converged value. MUST be a true rate, not a
  /// per-timestep accumulated mass - see the parameter description on "rate_postprocessor".
  const PostprocessorValue & _rate;

  /// Signed target mass rate (kg.s^-1); negative = injection, matching PorousFlow's own outflow
  /// sign convention (character < 0 => outflow < 0 => source).
  const Function & _target_rate;

  const Real _initial_pressure_step;
  const Real _max_pressure_change;
  const Real _min_pressure;
  const Real _max_pressure;
  const Real _relaxation_factor;
  const Real _min_pressure_difference;
  const Real _min_rate_difference;
  const Real _min_relative_error_for_floor;

  /// The bottomhole pressure this object currently reports - the value that will be in force for
  /// the timestep that is about to run. Restartable so a recover/restart resumes mid-correction
  /// rather than jumping back to initial_pressure.
  Real & _p_current;

  /// The bottomhole pressure that was in force during the *previous* timestep's solve, and the
  /// rate it delivered - the second point of the secant.
  Real & _p_prev;
  Real & _q_prev;

  /// Latest estimate of the well conductance B = -dQ/dP > 0 (kg.s^-1.Pa^-1). Zero means "no
  /// usable estimate yet", which triggers a deliberate probe step instead of a secant one.
  /// Persisted (rather than recomputed from scratch every timestep) so that one noisy or
  /// deadbanded pair of timesteps does not destroy an otherwise-good estimate.
  Real & _conductance;

  /// Whether _p_prev/_q_prev hold a real measurement yet
  bool & _have_history;

  /// Number of times execute() has run. The very first call happens at EXEC_TIMESTEP_BEGIN of
  /// the first timestep, before any solve has occurred - "rate_postprocessor" has not been
  /// computed from real physics yet at that point (it holds whatever default/bootstrap value it
  /// was declared with), so that first reading must never be recorded as a real measurement or
  /// compared against anything. See execute()'s implementation for why this matters.
  unsigned int & _call_count;

  /// The (signed) correction actually applied last call. Used only to detect whether the
  /// current call's correction is pushing the same direction (genuinely stuck, escalate) or the
  /// opposite direction (oscillating around a target it is already close to, do not escalate) -
  /// see execute()'s use of it alongside min_relative_error_for_floor.
  Real & _prev_step;
};
