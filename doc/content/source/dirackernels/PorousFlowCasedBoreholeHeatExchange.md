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
casing by the same sequence of weighted Dirac points as `PorousFlowPeacemanBorehole` (read from
the same `point_file`), active only where a `character` function (typically the complement of
the open-interval well's own `character`) is nonzero.

The wellbore fluid temperature is not a solved nonlinear variable. It is recomputed once per
residual and once per Jacobian evaluation from the closed-form solution of

\begin{equation}
\frac{dT_{\mathrm{well}}}{ds} = -\frac{hP}{\dot{m}c_p}\left(T_{\mathrm{well}} - \bar{T}_{\mathrm{formation}}\right) \ ,
\end{equation}

where $h$ is `heat_transfer_coefficient`, $P$ is the casing perimeter (from the `point_file`
radius), $\dot{m}$ is the total produced mass rate, $c_p$ is the fluid's specific heat, and
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
produced across the open interval (read from a `PorousFlowPointFluxQuantity`/
`PorousFlowPlotPointFluxQuantity` pair already reporting that well's per-point flux, at the same
one-time-step lag), not simply the local formation temperature at the open/cased boundary - fluid
produced deeper in the open interval is hotter, and most of the produced mass typically enters
there, so using only the boundary-point temperature would understate the fluid actually entering
the casing.

$h$ is nominally the *casing-side* resistance only (the in-well convective film, the steel
casing, and the cement sheath), referenced to the casing outer radius - not an overall
wellbore-to-far-field heat transfer coefficient, since the formation's own conductive resistance
is already resolved by the mesh. In practice, in this example, $h$ is capped far below any such
physical estimate by the cap's own deliberately very low permeability (chosen to keep the well's
pressure influence confined - see [geothermal_wellbore.md]): heat injected into the cap cannot
relieve via flow and instead thermally pressurizes the pore fluid there, and above roughly
$h=0.1\ \mathrm{W/m^2/K}$ that pressurization runs away and the solve diverges within the first
simulated day, regardless of how small a fraction of the advected heat rate it represents. The
value used, 0.01, was found by bisection to run stably for the full 5-year simulation; the
resulting effect is real but small (about 0.08% of the advected `well_heat_rate`).

As with `PorousFlowPeacemanBorehole`, the Jacobian only differentiates the local (same-point)
dependence of the residual on the current iterate; the wellbore temperature profile's dependence
on formation temperature at *other* well points cannot be assembled into by a single
`DiracKernel`, and (as above) is instead removed entirely by lagging to the old solution rather
than merely left undifferentiated.

## Example Input File Syntax

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=DiracKernels/wellbore_heat_exchange

!syntax parameters /DiracKernels/PorousFlowCasedBoreholeHeatExchange

!syntax inputs /DiracKernels/PorousFlowCasedBoreholeHeatExchange

!syntax children /DiracKernels/PorousFlowCasedBoreholeHeatExchange
