#include "PorousFlowRateControlledBoreholePressure.h"
#include "Function.h"

#include <algorithm>
#include <cmath>

registerMooseObject("FalconApp", PorousFlowRateControlledBoreholePressure);

InputParameters
PorousFlowRateControlledBoreholePressure::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();

  params.addRequiredParam<PostprocessorName>(
      "rate_postprocessor",
      "The well's own actual delivered signed mass rate (kg/s). This MUST be a true rate, not a "
      "mass accumulated over the timestep - PorousFlowSumQuantity/PorousFlowPlotQuantity "
      "(commonly named 'well_mass_rate' in this app's examples, despite the name) accumulates "
      "outflow*dt and is NOT a rate. Use a VectorPostprocessorReductionValue (value_type=sum) "
      "reducing a PorousFlowPlotPointFluxQuantity VectorPostprocessor instead - that one is "
      "explicitly documented as not multiplied by dt.");
  params.addRequiredParam<FunctionName>(
      "target_rate",
      "Signed target mass rate (kg/s); negative = injection, matching PorousFlow's own outflow "
      "sign convention. A Function (evaluated at the origin, since this is a single scalar "
      "target) rather than a plain Real so a startup ramp is expressible.");
  params.addRequiredParam<Real>(
      "initial_pressure",
      "The bottom_p_or_t value (Pa) reported on the very first timestep, before any rate/pressure "
      "history exists to base a correction on.");
  params.addRequiredRangeCheckedParam<Real>(
      "initial_pressure_step",
      "initial_pressure_step > 0",
      "Magnitude (Pa) of the deliberate probe step taken whenever no usable conductance estimate "
      "exists yet - the first correction, or any timestep whose rate response was too small/flat "
      "to refresh the estimate (eg the well sitting in Peaceman's zero-flux deadband).");
  params.addRangeCheckedParam<Real>(
      "max_pressure_change",
      1e7,
      "max_pressure_change > 0",
      "Per-timestep clamp on the magnitude of the pressure correction (Pa), so a bad or stale "
      "conductance estimate cannot move the pressure across the whole physical range in one "
      "step.");
  params.addParam<Real>(
      "min_pressure",
      -std::numeric_limits<Real>::max(),
      "Absolute lower clamp (Pa) on the reported pressure.");
  params.addParam<Real>(
      "max_pressure",
      std::numeric_limits<Real>::max(),
      "Absolute upper clamp (Pa) on the reported pressure.");
  params.addRangeCheckedParam<Real>(
      "relaxation_factor",
      1.0,
      "relaxation_factor > 0 & relaxation_factor <= 1",
      "Under-relaxation applied to each correction step (1 = no relaxation).");
  params.addRangeCheckedParam<Real>(
      "min_pressure_difference",
      1.0,
      "min_pressure_difference > 0",
      "Below this |delta pressure| between the last two timesteps, the conductance estimate is "
      "not refreshed (avoids dividing by a numerically negligible denominator).");
  params.addRangeCheckedParam<Real>(
      "min_rate_difference",
      1e-6,
      "min_rate_difference > 0",
      "Below this |delta rate| between the last two timesteps, the conductance estimate is not "
      "refreshed.");
  params.addRangeCheckedParam<Real>(
      "min_relative_error_for_floor",
      0.1,
      "min_relative_error_for_floor > 0 & min_relative_error_for_floor <= 1",
      "While the relative error |delta rate / target_rate| exceeds this fraction, the correction "
      "step is floored at 'initial_pressure_step' in magnitude, even if the secant-derived step "
      "would be smaller. A slope measured near one operating point can under-estimate the true "
      "correction needed once conditions have moved far enough that the well's real (non-affine) "
      "response curves away from that local slope - without this floor, extrapolating a "
      "locally-correct-but-too-shallow slope over a long distance leaves the controller stuck "
      "taking steps too small to make real progress for many timesteps.");

  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;
  params.addClassDescription(
      "Corrects a Peaceman borehole's 'bottom_p_or_t' pressure once per timestep, by a "
      "secant-method update on the well's own delivered mass rate, so that a rate-controlled "
      "(rather than pressure-controlled) well can be modelled without any change to "
      "PorousFlowPeacemanBorehole itself.");
  return params;
}

PorousFlowRateControlledBoreholePressure::PorousFlowRateControlledBoreholePressure(
    const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _rate(getPostprocessorValue("rate_postprocessor")),
    _target_rate(getFunction("target_rate")),
    _initial_pressure_step(getParam<Real>("initial_pressure_step")),
    _max_pressure_change(getParam<Real>("max_pressure_change")),
    _min_pressure(getParam<Real>("min_pressure")),
    _max_pressure(getParam<Real>("max_pressure")),
    _relaxation_factor(getParam<Real>("relaxation_factor")),
    _min_pressure_difference(getParam<Real>("min_pressure_difference")),
    _min_rate_difference(getParam<Real>("min_rate_difference")),
    _min_relative_error_for_floor(getParam<Real>("min_relative_error_for_floor")),
    _p_current(declareRestartableData<Real>("p_current", getParam<Real>("initial_pressure"))),
    _p_prev(declareRestartableData<Real>("p_prev", 0.0)),
    _q_prev(declareRestartableData<Real>("q_prev", 0.0)),
    _conductance(declareRestartableData<Real>("conductance", 0.0)),
    _have_history(declareRestartableData<bool>("have_history", false)),
    _call_count(declareRestartableData<unsigned int>("call_count", 0)),
    _prev_step(declareRestartableData<Real>("prev_step", 0.0))
{
  const auto & exec_flags = getParam<ExecFlagEnum>("execute_on");
  if (exec_flags.isValueSet(EXEC_LINEAR) || exec_flags.isValueSet(EXEC_NONLINEAR))
    paramError(
        "execute_on",
        "must not include LINEAR or NONLINEAR. The corrected pressure has to be a true constant "
        "throughout a timestep's whole Newton solve: PorousFlowPeacemanBorehole's Jacobian "
        "differentiates its residual with respect to the local porepressure only, treating "
        "bottom_p_or_t as fixed. Re-evaluating this correction inside the solve would introduce "
        "an undifferentiated, same-order-of-magnitude dependency - the same failure mode "
        "documented on PorousFlowCasedBoreholeHeatExchange's own old-solution lagging.");

  if (getParam<Real>("min_pressure") >= getParam<Real>("max_pressure"))
    paramError("max_pressure", "must be greater than 'min_pressure'.");
}

void
PorousFlowRateControlledBoreholePressure::execute()
{
  const Real q_meas = _rate;
  const Real q_target = _target_rate.value(_t, Point(0.0, 0.0, 0.0));
  const Real p_used = _p_current;
  const Real err = q_meas - q_target;

  if (_have_history)
  {
    const Real dp = p_used - _p_prev;
    const Real dq = q_meas - _q_prev;
    // The dq*dp < 0 check encodes the physically-required monotonicity (dQ/dP < 0: raising the
    // bottomhole pressure can only reduce production / increase injection magnitude). Requiring
    // it rejects noise-dominated and deadbanded pairs on top of the two magnitude floors below.
    if (std::abs(dp) > _min_pressure_difference && std::abs(dq) > _min_rate_difference &&
        dq * dp < 0.0)
      _conductance = -dq / dp;
    // otherwise: keep whatever estimate _conductance already holds (possibly still zero)
  }

  // The very FIRST call happens at EXEC_TIMESTEP_BEGIN of the first timestep, before any solve
  // has occurred, so q_meas on that one call is whatever bootstrap default "rate_postprocessor"
  // was declared with (typically 0) - not a real measurement of anything. Recording it as though
  // it reflected real physics would corrupt every conductance estimate derived from it
  // afterward (the second call would compute a slope by comparing this fake point against the
  // first REAL measurement, and - since a small early correction step yields only a small
  // pressure difference for the *next* comparison too - that one bad estimate can persist for
  // many subsequent calls before enough accumulated pressure change forces a refresh). So the
  // first call's q_meas is used only for choosing the initial probe direction below (which is
  // harmless: any target on the correct side of zero probes the right way), never recorded.
  if (_call_count > 0)
  {
    _p_prev = p_used;
    _q_prev = q_meas;
    _have_history = true;
  }
  ++_call_count;

  Real step;
  if (_conductance > 0.0)
    // Q(P_new) = q_target and Q(p_used) = q_meas, with Q affine in P at slope -_conductance,
    // gives P_new = p_used + (q_meas - q_target)/_conductance directly - no iteration needed
    // beyond what mobility's own weak pressure/temperature dependence introduces timestep to
    // timestep.
    step = err / _conductance;
  else
    // No usable slope yet: probe deliberately, in the same direction a real correction would
    // have gone.
    step = std::copysign(_initial_pressure_step, err);

  // Escalate the step, while the relative error exceeds min_relative_error_for_floor, but ONLY
  // if the correction has been pushing the SAME direction as last time - ie genuinely stuck,
  // making slow one-way progress against a target that has moved (or an initial guess that was
  // simply far off), as opposed to oscillating back and forth around a target it is already
  // close to. A secant slope measured near one operating point can under-estimate the true
  // correction needed once conditions have moved far enough that the well's real (non-affine)
  // response curves away from that local slope, leaving genuinely-stuck one-way drift
  // uncorrected for a long stretch of the run - but blindly escalating on relative error alone,
  // with no regard for whether the last correction already overshot, just turns an
  // already-adequate correction into a persistent overshoot/undershoot limit cycle instead.
  if (q_target != 0.0)
  {
    const Real relative_error = std::min(std::abs(err / q_target), 1.0);
    const bool same_direction_as_last_time = step * _prev_step > 0.0;
    if (relative_error > _min_relative_error_for_floor && same_direction_as_last_time)
    {
      const Real floor_step = relative_error * _max_pressure_change;
      if (std::abs(step) < floor_step)
        step = std::copysign(floor_step, err);
    }
  }

  step *= _relaxation_factor;
  step = std::copysign(std::min(std::abs(step), _max_pressure_change), step);
  _prev_step = step;

  _p_current = std::min(std::max(p_used + step, _min_pressure), _max_pressure);
}

Real
PorousFlowRateControlledBoreholePressure::getValue() const
{
  return _p_current;
}
