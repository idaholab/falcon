# PorousFlowCasedBoreholeHeatExchange

!syntax description /DiracKernels/PorousFlowCasedBoreholeHeatExchange

## Overview

[PorousFlowPeacemanBorehole.md] models heat exchange between a well and the formation
advectively: with `use_enthalpy = true`, its heat flux is the produced mass flux multiplied by
fluid enthalpy, so it is exactly zero wherever the mass flux is zero. That is correct over the
open (perforated) section of a well, where fluid genuinely crosses from the formation into the
well, but it leaves out an entire mechanism over a cased section: although no more mass crosses
the casing there, the produced fluid keeps flowing up the well, and the well loses or gains heat
*conductively* through the casing wall as it passes through the surrounding formation.

`PorousFlowCasedBoreholeHeatExchange` models that conductive exchange. It approximates the
casing by a sequence of weighted Dirac points, active only where a `character` function
(typically the complement of the open-interval well's own `character`) is nonzero. Unlike
`PorousFlowPeacemanBorehole`, that point set need not come from the same hand-listed `point_file`
as the open interval's mass-extraction kernels - [PolylineDiracPoints.md] auto-places one point
per mesh element crossed between a set of literal waypoint coordinates, so the cased section's
own points can sit at the mesh's own resolution rather than at whatever spacing the open
interval's Peaceman points happen to use. See "Point set and the open/cased boundary" below for
the one geometric requirement this places on that point set.

The wellbore fluid temperature is not a solved nonlinear variable. It is recomputed once per
residual and once per Jacobian evaluation from the closed-form solution of

\begin{equation}
\frac{dT_{\mathrm{well}}}{ds} = -\frac{hP}{\dot{m}c_p}\left(T_{\mathrm{well}} - \bar{T}_{\mathrm{formation}}\right) \ ,
\end{equation}

where $h$ is `heat_transfer_coefficient`, $P$ is the casing perimeter (from each point's own
weight, ie its radius), $\dot{m}$ is the total produced mass rate, $c_p$ is the fluid's specific
heat, and
$\bar{T}_{\mathrm{formation}}$ is a single length-weighted average formation temperature over
the whole cased section (rather than a segment-by-segment recomputation): $T_{\mathrm{well}}$ at
any point depends only on the entry temperature below and that point's own cumulative distance
from the boundary, never on another cased point's own $T_{\mathrm{well}}$.

Critically, every temperature this kernel samples - the formation temperature used to build
$\bar{T}_{\mathrm{formation}}$, and (indirectly, via the mixing temperature below) the entry
condition - is read from the *old* (previous time step's converged) solution, not the current
nonlinear iterate, via `SystemBase::solutionOld()`. This makes $T_{\mathrm{well}}$ a true
constant throughout a given time step's whole Newton solve, so `computeQpJacobian()`'s
local-only derivative (treating $T_{\mathrm{well}}$ as fixed while differentiating the
residual's one remaining dependence on the current iterate, at this same point) is *exact* for
that solve, not an approximation of an otherwise-undifferentiated dependency. An earlier version
sampled the current iterate directly (mirroring `PorousFlowPeacemanBorehole`'s own
`unit_weight_temperature`); unlike that class's smoothly-varying, weakly temperature-dependent
hydrostatic pressure integral, $T_{\mathrm{well}}$'s dependence on formation temperature
elsewhere along the well can be just as large as the one term the Jacobian captures, and left
undifferentiated this destabilized the coupled Newton solve during the example's production
ramp-up - lagging to the old solution removes the dependency instead of merely approximating it.

The starting (entry) condition is the mass-flux-weighted mixing temperature of everything
produced across the open interval, read from a `PorousFlowPointFluxQuantity`/
`PorousFlowPlotPointFluxQuantity` pair already reporting that well's per-point flux *and*
coordinates (at the same one-time-step lag), not simply the local formation temperature at the
open/cased boundary - fluid produced deeper in the open interval is hotter, and most of the
produced mass typically enters there, so using only the boundary-point temperature would
understate the fluid actually entering the casing. Sampling formation temperature at that VPP's
own reported coordinates, rather than at this kernel's own points, is what lets the two point
sets be placed completely independently of one another.

## Point set and the open/cased boundary

The wellbore-temperature march needs a point right at the open/cased boundary to enter the
mixing temperature at and to measure cumulative distance from - so this kernel's own *last*
(deepest) point must be one where `character` is zero. `initialSetup()` checks this and
`paramError`s if it isn't: without the check, a point set that stopped even slightly short of the
boundary would leave every point active and this kernel would silently exchange no heat at all,
with no diagnostic. By convention that last point should coincide with the shallowest point of
the open interval's own point set, so that `s = 0` in the ODE above sits at the true physical
boundary - but nothing enforces that coordinate match itself, since the mixing temperature no
longer depends on this kernel's own points at all (see above).

$h$ is nominally the *casing-side* resistance only (the in-well convective film, the steel
casing, and the cement sheath), referenced to the casing outer radius - not an overall
wellbore-to-far-field heat transfer coefficient, since the formation's own conductive resistance
is already resolved by the mesh. In this example, the cap's permeability is realistic (see
[geothermal_wellbore.md]), so $h$ can use a realistic estimate too: 1.5 $\mathrm{W/m^2/K}$. A
separate numerical limit, unrelated to any thermal-pressurization concern, was found at the
point spacing this kernel originally used (a hand-listed `point_file` shared with the open
interval, 100m between points): above roughly $h=1.5\text{-}1.8\ \mathrm{W/m^2/K}$ the solve
stalled near day 156 of the 5-year run. Switching to [PolylineDiracPoints.md]'s finer,
mesh-resolution point placement removed that wall - the example now runs cleanly through at
least $h=3\ \mathrm{W/m^2/K}$, only failing somewhere between 3 and 5. The resulting conductive
exchange totals about 9% of the advected `well_heat_rate` - real, but small next to a much larger
effect: at this permeability, the cap itself is porous enough for the well's own pressure
drawdown to drive substantial advective flow up through it, which dominates the temperature
changes this example actually shows far more than this conductive term does.

As with `PorousFlowPeacemanBorehole`, the Jacobian only differentiates the local (same-point)
dependence of the residual on the current iterate; the wellbore temperature profile's dependence
on formation temperature at *other* well points cannot be assembled into by a single
`DiracKernel`, and (as above) is instead removed entirely by lagging to the old solution rather
than merely left undifferentiated.

## Example Input File Syntax

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=Reporters/cased_path

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=DiracKernels/wellbore_heat_exchange

!syntax parameters /DiracKernels/PorousFlowCasedBoreholeHeatExchange

!syntax inputs /DiracKernels/PorousFlowCasedBoreholeHeatExchange

!syntax children /DiracKernels/PorousFlowCasedBoreholeHeatExchange
