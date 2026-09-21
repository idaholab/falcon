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
    _p_current(declareRestartableData<Real>("p_current", getParam<Real>("initial_pressure"))),
    _p_prev(declareRestartableData<Real>("p_prev", 0.0)),
    _q_prev(declareRestartableData<Real>("q_prev", 0.0)),
    _conductance(declareRestartableData<Real>("conductance", 0.0)),
    _have_history(declareRestartableData<bool>("have_history", false))
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

  _p_prev = p_used;
  _q_prev = q_meas;
  _have_history = true;

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

  step *= _relaxation_factor;
  step = std::copysign(std::min(std::abs(step), _max_pressure_change), step);

  _p_current = std::min(std::max(p_used + step, _min_pressure), _max_pressure);
}

Real
PorousFlowRateControlledBoreholePressure::getValue() const
{
  return _p_current;
}
