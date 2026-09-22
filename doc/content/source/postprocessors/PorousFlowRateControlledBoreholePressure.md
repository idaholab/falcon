# PorousFlowRateControlledBoreholePressure

!syntax description /Postprocessors/PorousFlowRateControlledBoreholePressure

## Description

`PorousFlowRateControlledBoreholePressure` lets a [PorousFlowPeacemanBorehole.md] well be driven
by a prescribed flow *rate* instead of a prescribed bottomhole pressure, without any change to
`PorousFlowPeacemanBorehole` itself: it reports the `bottom_p_or_t` value that well should use
this timestep, corrected by a secant-method update on the well's own delivered mass rate.

### Why this works without ever touching permeability

Peaceman's flux at each point is $\mathrm{WI}_i \lambda_i (P_i - P_{\mathrm{well},i})$, and
$P_{\mathrm{well},i} = P_{\mathrm{bot}} + (\text{a per-point hydrostatic/thermal offset
independent of } P_{\mathrm{bot}})$. So for a momentarily-frozen permeability/mobility state, the
*total* well rate is an affine function of the single scalar $P_{\mathrm{bot}}$:

\begin{equation}
\dot m_{\mathrm{total}}(P_{\mathrm{bot}}) = A - B P_{\mathrm{bot}}, \qquad
B = \sum_i \mathrm{WI}_i \lambda_i \ ,
\end{equation}

where $B$ is the well's conductance. A plain secant update, using only two already-available
scalars - the well's own delivered rate and its own `bottom_p_or_t` history - finds the pressure
that would deliver a target rate:

\begin{equation}
B^{(n)} = -\frac{\dot m^{(n)} - \dot m^{(n-1)}}{P^{(n)} - P^{(n-1)}}, \qquad
P^{(n+1)} = P^{(n)} + \omega \frac{\dot m^{(n)} - \dot m_{\mathrm{target}}}{B^{(n)}} \ ,
\end{equation}

with $\omega$ = `relaxation_factor`. Crucially this never evaluates $\mathrm{WI}_i$ (hence never
needs permeability) at all - which matters, because $\mathrm{WI}_i$ depends on a
`MaterialProperty` that is only valid inside the normal per-quadrature-point Dirac-kernel
evaluation loop. There is no way to assemble an *analytic* conductance from a
`residualSetup()`-style calculation done before any point has been visited - unlike, say,
[PorousFlowCasedBoreholeHeatExchange.md]'s own wellbore-temperature profile, which only ever
needs to sample a solved field (via `System::point_value()`), not a material property.

One consequence follows immediately from reusing Peaceman's own physics this way: once a single
$P_{\mathrm{bot}}$ is chosen, the delivered flow partitions itself across the well's open-interval
points in proportion to each point's own $\mathrm{WI}_i \lambda_i$ - which already depends on
local permeability. This partitioning is *emergent*, not something this object computes or
imposes.

### `execute_on`

This object must run at `EXEC_TIMESTEP_BEGIN` only (the default; setting `LINEAR` or `NONLINEAR`
is an error). That keeps the pressure it reports a true constant through the entire following
timestep's Newton solve - the same "frozen for the whole timestep" property that makes
[PorousFlowCasedBoreholeHeatExchange.md]'s own old-solution lagging of the wellbore temperature
exact rather than approximate.

### Units trap

`rate_postprocessor` must be a true rate (kg/s). `PorousFlowSumQuantity`/`PorousFlowPlotQuantity`
(commonly named `well_mass_rate` in this app's own examples, despite the name) accumulates
`outflow*dt` over the timestep - a mass, not a rate - and must not be used here. Reduce a
`PorousFlowPlotPointFluxQuantity` VectorPostprocessor with a `VectorPostprocessorReductionValue`
(`value_type = sum`) instead; that VPP's `flux` column is explicitly documented as not multiplied
by `dt`.

### Convergence behavior

Because the secant needs two data points, the reported pressure does not reach the target rate
instantly - it settles over some number of timesteps (a probe step on the first correction, then
secant corrections once a conductance estimate exists). This is a modelling characteristic, not
an approximation error once settled: a real rate-controlled well's bottomhole pressure also takes
some time to reach its operating point. Two things determine how *many* timesteps that takes -
and, in this app's own examples, `IterationAdaptiveDT` grows `dt` aggressively, so a slow
convergence in timestep count can mean a slow convergence in a large fraction of the run's
simulated time:

- **`initial_pressure` should be calibrated, not guessed.** A short fixed-`bottom_p_or_t` run of
  the same model (varying only that one value) directly measures what pressure the target rate
  actually needs at the model's own initial state - starting from that value means the controller
  has almost nothing left to correct from step one onward, rather than spending many timesteps
  (and, under a growing `dt`, a large fraction of simulated time) closing a big initial gap.
- **The well's response is not perfectly affine over a wide pressure range.** Real mobility
  evolves as the near-well temperature/pressure state evolves (eg a cold injectate advancing, or
  reservoir pressure itself rising from the injected mass), so a secant slope measured near one
  operating point can under-estimate the correction needed once conditions - or the required
  pressure - have moved far enough away from where that slope was measured. Left unchecked, this
  can leave the reported pressure crawling toward the target far more slowly than the safety
  clamp `max_pressure_change` would otherwise allow.

`min_relative_error_for_floor` addresses the second point: while the relative error
$|\dot m - \dot m_{\mathrm{target}}| / |\dot m_{\mathrm{target}}|$ exceeds this fraction, the
correction is floored at (that same relative error) $\times$ `max_pressure_change`, rather than
whatever smaller value the secant alone would have produced - but *only* when the correction is
pushing the **same direction** as the previous call's. That direction check is what keeps this
escalation from causing an oscillation: a controller already close to its target, and merely
bouncing back and forth across it (eg through Peaceman's own zero-flux deadband), has a
correction direction that *alternates* call to call, and is left alone; one that has been making
slow, one-directional progress for many calls without closing the gap is not, and gets a bigger
push. So `max_pressure_change` is not purely a safety-only ceiling here - it also sets the size
of this escalated correction, and should be calibrated to the well's own true conductance rather
than left at an arbitrarily generous value (an oversized `max_pressure_change` can itself induce
the very oscillation the direction check exists to detect, once the escalated step is large
enough to overshoot in a single call).

The very first call to `execute()` happens at `EXEC_TIMESTEP_BEGIN` of the very first timestep,
before any solve has occurred - `rate_postprocessor` has not been computed from real physics yet
at that point, so that reading is used only to pick the first probe's direction and is never
recorded as a measurement (recording it would corrupt the very first real conductance estimate,
computed from comparing it against the first genuine post-solve rate).

## Example Input Syntax

!listing examples/geothermal_wellbore/wells_injection.i block=Postprocessors/bhp_control

!syntax parameters /Postprocessors/PorousFlowRateControlledBoreholePressure

!syntax inputs /Postprocessors/PorousFlowRateControlledBoreholePressure

!syntax children /Postprocessors/PorousFlowRateControlledBoreholePressure
