# Geothermal well production, rate-controlled injection, and wellbore-reservoir heat exchange

## Introduction

This example has three goals: to show how a temperature-dependent fluid density changes the
pressure needed to produce hot water from a geothermal well; to show the two distinct mechanisms
by which a well exchanges heat with the rock around it - advectively, where fluid actually
crosses into the well, and conductively, through the casing wall above that; and to show how a
well can be driven by a prescribed *flow rate* rather than a prescribed pressure, which in turn
lets the injected flow partition itself across the open interval according to local permeability.

### Thermal fluid production

A geothermal production well draws hot water up from depth, and the pressure needed to lift
that column depends on how dense the water is along the way. [PorousFlowPeacemanBorehole.md]
has traditionally built this wellbore pressure profile from a single constant fluid unit weight
$\gamma$ (density times gravity):

\begin{equation}
P_{\mathrm{wellbore}}(x_{i}) = P_{\mathrm{bot}} + \gamma \cdot (x_{i} - x_{i}^{\mathrm{bot}}) \ ,
\end{equation}

which is exact only if the fluid's density doesn't change along the well. In a geothermal well
with a large thermal gradient, it does - water near the bottom of a deep well can be
significantly less dense than water near the top, so a single $\gamma$ is only ever right at
whichever depth it was calibrated for.

This example demonstrates the alternative: `unit_weight_fp`, which integrates a
temperature-dependent fluid density along the well instead (see
[porous_flow/sinks.md#the-wellbore-pressure-with-a-temperature-dependent-fluid-density] for the
full formula), together with per-point flux reporting via [PorousFlowPointFluxQuantity.md] and
[PorousFlowPlotPointFluxQuantity.md].

### Borehole heat exchange

The well is cased through the cap and open to the formation only over its bottom 1000m (see
[geothermal_wellbore_geometry_fig]), and it exchanges heat with the rock two different ways
depending on whether mass can actually cross the well wall there:

- +Open interval:+ wherever fluid physically crosses from the formation into the well, it
  carries its own enthalpy with it - a second [PorousFlowPeacemanBorehole.md], with
  `use_enthalpy = true`, models exactly this:

  \begin{equation}
  Q_{\mathrm{adv}} = \dot{m} \, h_{\mathrm{fluid}} \ ,
  \end{equation}

  the produced mass rate $\dot{m}$ times the fluid's own enthalpy per unit mass
  $h_{\mathrm{fluid}}$. This is the correct, and only, mechanism for well-to-formation heat
  exchange available in the PorousFlow module on its own, since it is advective only - it is
  exactly zero wherever $\dot{m}$ is zero, which is everywhere above the open interval.
- +Cased section:+ above the open interval, casing seals the well off from the formation, so
  no more mass crosses the well wall - but the produced fluid keeps flowing up through the cased
  section, and still loses or gains heat *conductively* through the casing wall as it passes
  through the surrounding cap. [PorousFlowCasedBoreholeHeatExchange.md] models this by deriving
  a wellbore fluid temperature $T_{\mathrm{well}}$ (not a solved variable) from

  \begin{equation}
  \frac{dT_{\mathrm{well}}}{ds} = -\frac{hP}{\dot{m}c_{p}}
  \left(T_{\mathrm{well}} - \bar{T}_{\mathrm{formation}}\right) \ ,
  \end{equation}

  where $s$ is distance up from the open/cased boundary, $h$ is `heat_transfer_coefficient`, $P$
  is the casing perimeter, and $\bar{T}_{\mathrm{formation}}$ is a single, length-weighted
  average formation temperature over the whole cased section - not solved point by point, since
  a single `DiracKernel` cannot assemble a Jacobian entry that couples one well point's
  temperature to another's (the same limitation [PorousFlowPeacemanBorehole.md] documents for
  its own `unit_weight_fp`). This has the closed-form solution

  \begin{equation}
  T_{\mathrm{well}}(s) = \bar{T}_{\mathrm{formation}}
  + \left(T_{\mathrm{mix}} - \bar{T}_{\mathrm{formation}}\right)
  \exp\left(-\frac{hPs}{\dot{m}c_{p}}\right) \ ,
  \end{equation}

  starting from $T_{\mathrm{mix}}$, the mass-flux-weighted mixing temperature of everything
  produced across the open interval below:

  \begin{equation}
  T_{\mathrm{mix}} = \frac{\sum_{i} \dot{m}_{i} T_{i}}{\sum_{i} \dot{m}_{i}} \ ,
  \end{equation}

  where the sum runs over the open interval's Peaceman points, $\dot{m}_{i}$ is the mass flux
  reported at point $i$, and $T_{i}$ the local formation temperature there - fluid produced
  deeper in the open interval is hotter, and most of the produced mass typically enters there,
  so this mixing average (not just the formation temperature at the open/cased boundary) is the
  physically correct starting condition. $h$, $\bar{T}_{\mathrm{formation}}$, $T_{\mathrm{mix}}$
  and $c_{p}$ (the fluid's specific heat) are all sampled from the *previous* time step's
  converged solution, not the current nonlinear iterate - see
  [PorousFlowCasedBoreholeHeatExchange.md] for why.

  $h$ represents the casing's own thermal resistance (steel, cement, the in-well film), not the
  formation's, which is already resolved by the mesh, and is set to a realistic estimate of
  1.5 $\mathrm{W/m^2/K}$. An earlier revision of this example used a deliberately very low cap
  permeability, under which heat injected here could not relieve via flow and instead thermally
  pressurized the trapped pore fluid, running away above $h \sim 0.1$; at the realistic cap
  permeability used now (see [#model-setup]), that specific failure mode no longer binds. A
  separate numerical limit was found at the point placement then in use (a hand-listed
  `point_file` shared with the open interval, 100m between points): the simulation stalled
  around day 156 of the run above roughly $h \sim 1.5\text{-}1.8\ \mathrm{W/m^2/K}$. Switching to
  the auto-placed points described next removed that wall - this revision now runs cleanly
  through at least $h = 3\ \mathrm{W/m^2/K}$, twice the value actually used, only failing
  somewhere between 3 and 5. The resulting conductive exchange totals about 9% of the advected
  heat rate $Q_{\mathrm{adv}}$ over the run - real, but secondary to a much larger effect: at
  this permeability, the cap itself is porous enough for the well's own pressure drawdown to
  drive significant advective flow up through it, which dominates the temperature changes shown
  in [#results] far more than this conductive term does.

  [PorousFlowCasedBoreholeHeatExchange.md]'s own Dirac points are placed by
  [PolylineDiracPoints.md] (see the `[Reporters][cased_path]` block in [#model-setup]), not by a
  hand-listed `point_file`: given just the wellhead and the open/cased boundary as literal
  coordinates, it auto-finds every mesh element the path between them crosses and places one
  point per element, at this mesh's own 50m vertical resolution rather than at the open
  interval's 100m Peaceman spacing. An earlier revision shared the open interval's `point_file`
  for this kernel too, which resolved the conductive exchange on only every other mesh node - a
  visibly lumpy pattern in Exodus output, and (as the day-156 wall above shows) a real numerical
  cost as well, not just a cosmetic one. The same object accepts three or more waypoints for a
  bent or branching well and works unchanged on a fully 3D Cartesian mesh; this example's well is
  vertical and this mesh is 2D RZ only because the physics being demonstrated is.

### Rate-controlled injection

The production well above is driven by a prescribed pressure - `bottom_p_or_t` - with the
delivered mass rate emerging from [PorousFlowPeacemanBorehole.md]'s own well-index (WI) formula.
An injection well is often better specified the other way around: a target flow rate, with the
pressure needed to deliver it left to emerge. [PorousFlowRateControlledBoreholePressure.md]
provides exactly this, without any change to `PorousFlowPeacemanBorehole` itself: because its
flux at each point is $\mathrm{WI}_i \lambda_i (P_i - P_{\mathrm{well},i})$, and
$P_{\mathrm{well},i}$ is an affine function of the single scalar `bottom_p_or_t`, the *total*
well rate is itself affine in that one scalar:

\begin{equation}
\dot{m}_{\mathrm{total}}(P_{\mathrm{bot}}) = A - B P_{\mathrm{bot}}, \qquad
B = \sum_i \mathrm{WI}_i \lambda_i \ ,
\end{equation}

so a secant-method correction, using only the well's own delivered rate and its own pressure
history - never permeability directly - finds the pressure that delivers a target rate:

\begin{equation}
B^{(n)} = -\frac{\dot m^{(n)} - \dot m^{(n-1)}}{P^{(n)} - P^{(n-1)}}, \qquad
P^{(n+1)} = P^{(n)} + \frac{\dot m^{(n)} - \dot m_{\mathrm{target}}}{B^{(n)}} \ .
\end{equation}

The consequence that motivates this example: once a single $P_{\mathrm{bot}}$ is chosen, flow
partitions itself across the open interval's points in proportion to each point's own local
well index - which already depends on local permeability -

\begin{equation}
\frac{\dot m_i}{\dot m_{\mathrm{total}}} = \frac{\mathrm{WI}_i \lambda_i}{\sum_j \mathrm{WI}_j \lambda_j} \ ,
\end{equation}

*emergently*, not as something explicitly computed or imposed. [#results] shows this directly: a
10x-higher-permeability sub-zone carved out of part of the open interval receives roughly 9x more
flow than its neighbors, using nothing but the rate controller and the mesh's own material
properties.

Injected heat enters advectively too, but the fluid being pushed in carries its *own* prescribed
enthalpy, not the local reservoir's - the opposite of `use_enthalpy = true`'s assumption for a
producer. [PorousFlowPeacemanEnthalpySink.md] provides this: the same Peaceman mass flux,
multiplied by $h_{\mathrm{fluid}}(p, T_{\mathrm{inj}})$ at a prescribed injection temperature
rather than the local solved field's temperature.

The cased section above the open interval still conducts heat between the well and the cap, via
the same [PorousFlowCasedBoreholeHeatExchange.md] as the production well - but with
`flow_direction = injection`: fluid enters at the wellhead at `injection_temperature` and the
march runs downward to the open/cased boundary, the mirror image of production's upward march
from the boundary.

+Known simplifications:+ `injection_temperature` is the raw temperature at the wellhead, not fed
back from the cased kernel's own computed arrival temperature at the open/cased boundary - a
secondary effect, in the same spirit as this example's own note above that the conductive term is
secondary (real geothermal wells' casing does affect the injectate's temperature on the way down,
but by less than the other simplifications already in this model). `use_mobility` still weights
by the *reservoir-side* mobility, not the cooler injected fluid's own higher viscosity, so the
delivered rate at a given overpressure is somewhat overstated. And the rate converges over a
handful of timesteps rather than instantly - itself physically reasonable (a real rate-controlled
well's bottomhole pressure also takes time to settle), not an error in the settled physics.

## Model setup

The well has radial symmetry, so it is simulated using RZ coordinates - the same approach used
by moose's own [ates.md] example:

!listing examples/geothermal_wellbore/model_common.i block=Mesh

which means gravity acts along what is usually thought of as the "y" direction:

!listing examples/geothermal_wellbore/model_common.i block=GlobalParams

The geometry is shown in [geothermal_wellbore_geometry_fig]: a 1000m-radius, 2000m-deep domain,
with a lower-permeability cap over the top 1000m and the reservoir below it. The well is cased
through the cap and open to the formation only over its bottom 1000m - so casing coincides with
the cap and the open completion with the reservoir, matching conventional well design. On the
well line itself, dashed marks the open interval, where fluid can actually cross into the well,
and solid marks the cased section, which is sealed to mass flow. The white dots along the open
interval are the 11 discrete Peaceman points (from `geothermal_wellbore.bh`, 100m apart) where
`withdraw_fluid`/`withdraw_heat` actually act - the only locations where fluid leaves the
formation and enters the well. The cased section above them has its own, separately-placed set
of 20 points (one per 50m mesh element, via [PolylineDiracPoints.md]; see
[#borehole-heat-exchange]) where the conductive heat-exchange kernel acts - not shown here, since
they are simply the mesh's own resolution rather than a deliberate modelling choice the way the
Peaceman points are.

!media geothermal_wellbore_schematic.png
  id=geothermal_wellbore_geometry_fig
  style=width:80%;margin-left:auto;margin-right:auto;
  caption=Geometry on a radial slice of the geothermal wellbore model. Dashed: open interval
  (fluid can cross into the well); solid: cased section (sealed to mass flow, conductive heat
  exchange only). White dots: the discrete Peaceman points where fluid leaves the formation.

A geothermal gradient (30 degC at the surface, 100 degC/km) sets the initial and far-field
boundary temperature, with a matching hydrostatic pressure profile:

!listing examples/geothermal_wellbore/model_common.i start=[Functions] end=[ICs]

The core physics is handled by the [PorousFlowFullySaturated.md] Action:

!listing examples/geothermal_wellbore/model_common.i block=PorousFlowFullySaturated

Cap and reservoir properties are realistic geologic values - permeability ~1E-16 m$^2$ for the
cap and ~1E-15 m$^2$ for the reservoir, isotropic thermal conductivity ~2.5 $\mathrm{W/m/K}$,
and a rock specific heat of 800 $\mathrm{J/kg/K}$ in both layers - not idealized for visual
effect the way an earlier revision of this example set them. The reservoir's permeability is
still kept about 10x below a typical ~1E-14 m$^2$ value, a modest, deliberate choice so the
well's pressure influence doesn't diffuse across the whole domain within the runnable 5-year
timescale; the cap's permeability is fully realistic. One direct consequence, visible throughout
[#results]: a realistically-permeable cap lets the well's own pressure drawdown drive real
advective flow up through it, which turns out to dominate the cap's thermal response far more
than either the conductive heat exchange described above or simple conduction alone would:

!listing examples/geothermal_wellbore/model_common.i block=Materials

The mass-extraction DiracKernels (`withdraw_fluid`/`withdraw_heat`) share one `point_file` and
`character` function, so the open interval is identical for both. The cased-section heat-exchange
kernel uses the same well line and the complementary `character` function, but places its own
points via the `[Reporters][cased_path]` block below rather than sharing that `point_file`:

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=Reporters/cased_path

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=DiracKernels

`production.i` uses this well definition; `production_constant_unit_weight.i` swaps in
`wells_constant_unit_weight.i` instead, which is otherwise identical except for a single
constant `unit_weight` (see that file for how it was derived) in place of `unit_weight_fp` -
everything else in the two input files, down to the shared `model_common.i`, is the same.

### Injection well

`injection.i` shares `model_common.i` unchanged (so the production examples' own results above
are unaffected), and additionally includes `injection_zones.i`, which carves a 10x
higher-permeability sub-zone out of part of the open interval (y=-1400 to -1600, entirely within
the reservoir block) - see [#rate-controlled-injection] for why:

!listing examples/geothermal_wellbore/injection_zones.i

[geothermal_wellbore_injection_geometry_fig] shows this well's geometry - otherwise identical to
[geothermal_wellbore_geometry_fig] above, with the addition of that sub-zone and the direction of
flow at the open interval.

!media geothermal_wellbore_injection_schematic.png
  id=geothermal_wellbore_injection_geometry_fig
  style=width:80%;margin-left:auto;margin-right:auto;
  caption=Geometry on a radial slice of the rate-controlled injection well. Shaded band: the
  10x-higher-permeability sub-zone carved out of the reservoir by `injection_zones.i`. Blue arrow:
  direction of flow at the open interval (into the formation, the opposite of
  [geothermal_wellbore_geometry_fig]'s production well).

`wells_injection.i` defines the well itself: `inject_fluid` and `inject_heat` share the same
`point_file`/`character` as the production wells' own mass-extraction kernels (just with the
opposite-signed `character`), and `wellbore_heat_exchange` places its own points the same way as
the production well's - the only genuinely new pieces are the rate controller and its supporting
`Functions`:

!listing examples/geothermal_wellbore/wells_injection.i block=Functions

!listing examples/geothermal_wellbore/wells_injection.i block=DiracKernels

!listing examples/geothermal_wellbore/wells_injection.i block=Postprocessors/bhp_control

## Running the Example

```
falcon-opt -i production.i                          # the new unit_weight_fp behaviour
falcon-opt -i production_constant_unit_weight.i      # the old constant-unit_weight behaviour
falcon-opt -i injection.i                             # the rate-controlled injection well
python3 plot_results.py                              # produces the plots below
python3 make_schematic.py                             # produces the geometry schematic above
```

## Results

### Production

[geothermal_wellbore_rates_fig] shows the extracted mass and heat rate over the 5-year run, for
both wellbore-pressure treatments. Over the full run, the old constant-`unit_weight` treatment
extracts about 17% more mass and heat than the new temperature-dependent treatment - a
substantial difference, driven by the 100 K range spanned by this well's open interval. The old
treatment's constant was itself a reasonable single-point estimate (density at the open
interval's mean depth conditions); this illustrates the practical benefit of `unit_weight_fp` -
it removes the need to hand-derive and re-check that constant every time a well's design changes.

!media geothermal_wellbore_rates_vs_time.png
  id=geothermal_wellbore_rates_fig
  style=width:70%;margin-left:auto;margin-right:auto;
  caption=Extracted mass rate (top) and heat rate (bottom) vs. time, old (constant
  `unit_weight`) vs. new (`unit_weight_fp`).

[geothermal_wellbore_pressure_fig] shows exactly where that difference comes from: pressure vs.
depth along the well's open (feed) interval, y=-1000 to -2000, at the end of the run, for both
treatments, plus their difference - the cased section above carries no mass at all in either
treatment, so it adds nothing to this particular comparison. The two profiles diverge smoothly
moving up from the well bottom, reaching about 153 kPa at the interval's midpoint (y=-1500)
before narrowing back to under 1 kPa at y=-2000 - not exactly zero, because
[PorousFlowPeacemanBorehole.md]'s Peaceman coupling is a finite-well-index source term, not a
hard Dirichlet constraint on `bottom_p_or_t`: both treatments solve to about 18.05-18.06 MPa
there, close to, but not exactly onto, the `bottomhole_pressure` Function's own 18.02 MPa value.
At the top of the feed zone (y=-1000) the difference has grown to about 64 kPa - the same
propagation-through-the-permeable-cap effect described in [#model-setup] and shown in
[geothermal_wellbore_temperature_depths_fig], now visible from the pressure side instead of the
temperature side.

!media geothermal_wellbore_pressure_depth.png
  id=geothermal_wellbore_pressure_fig
  style=width:95%;margin-left:auto;margin-right:auto;
  caption=Left: pressure vs. depth along the well's open interval (depth -1000 to -2000 m), final
  step, old vs. new. Right: their difference vs. depth.

[geothermal_wellbore_temperature_depths_fig] shows absolute temperature vs. time at a series of
depths along the well axis, for the `unit_weight_fp` treatment - each curve starts at that
depth's own undisturbed background value on the geothermal gradient (303 K at the surface, up to
503 K at the well bottom), and the picture of how they then evolve is very different from a
purely conductive cap: temperature rises substantially throughout the cased section, not just
near its two ends. At the surface (y=0m) it rises by about 46 K over 5 years, ending near 349 K;
at y=-200, -400, -600 and -800m the rises are about 25, 21, 17 and 14 K respectively - a smooth
gradient, largest near the surface and decreasing with depth through the cap. This is not the
new conductive heat exchange at work (which totals only about 9% of the advected heat rate): it
is the realistically-permeable cap itself allowing the well's pressure drawdown to pull fluid,
and the heat it carries, up through the cased section by advection - a genuinely different, and
much larger, mechanism than the small conductive term derived in [#borehole-heat-exchange]. Right
at the cap/reservoir interface (y=-1000) the change is negligible (a fraction of a degree either
way), and below it - in the open interval and reservoir - the picture reverts to what drives the
mass/heat extraction comparison above: temperature at the well bottom (y=-2000m) falls by about
2.4 K as continuous heat extraction outpaces replenishment from the surrounding reservoir, while
y=-1500m barely changes at all.

These near-surface numbers are noticeably smaller than an earlier revision of this example
reported at the same materials and `heat_transfer_coefficient` (about 67 K at the surface, rather
than 46 K) - a direct, and reassuring, consequence of switching to
[PolylineDiracPoints.md]'s finer, mesh-resolution point placement described in
[#borehole-heat-exchange]. A coarser point spacing concentrates the same total exchanged heat
onto fewer Dirac points, and each point's own local Dirac-source response in the FEM solution
overshoots more the more concentrated it is; the earlier, coarser numbers were partly this
overshoot, not purely the physical advective signal. The total conductive exchange itself barely
moved (see [#borehole-heat-exchange]) - only its spatial distribution, and hence how much
discretization artifact leaks into a point sample taken right at the well, did.

!media geothermal_wellbore_temperature_depths.png
  id=geothermal_wellbore_temperature_depths_fig
  style=width:80%;margin-left:auto;margin-right:auto;
  caption=Absolute temperature vs. time at depths along the well axis, `unit_weight_fp`
  treatment.

[geothermal_wellbore_cap_temperature_fig] shows a radial temperature profile strictly *inside*
the cap, 500m below the surface, at a handful of times through the run, zoomed to the nearest
50m of the well - the whole signal decays back to background within about that distance, so a
wider view would only add empty axis (`model_common.i`'s `cap_temperature_radial` sampler was
tightened to 2m spacing over the nearest 100m specifically so this zoom stays smooth rather than
visibly cropped). The profile is flat at the undisturbed background (353.15 K, the geothermal
gradient's value at this depth) at early times, then a growing peak develops right at the well -
reaching about 372 K by the end of the run, 19 K above background. This is the same advective
mechanism visible in [geothermal_wellbore_temperature_depths_fig], now shown as a function of
radius rather than time: hot fluid drawn up along the well axis through the realistically
-permeable cap, not the new conductive heat exchange kernel, whose own contribution (reported
separately via `wellbore_heat_point_flux_out`, not distinguishable from the advective signal in a
plain temperature snapshot like this one) is an order of magnitude smaller. The same radial
signature appears at the cap/reservoir interface itself (y=-1000) and at every other depth
through the cased section - materially the same story throughout, so it is not plotted
separately at each one.

!media geothermal_wellbore_cap_temperature_radial.png
  id=geothermal_wellbore_cap_temperature_fig
  style=width:65%;margin-left:auto;margin-right:auto;
  caption=Temperature vs. radius (zoomed to the nearest 50m) at 500m depth within the cap, at
  several times through the run, the `unit_weight_fp` treatment.

### Injection

[geothermal_wellbore_injection_rate_fig] shows the delivered injection rate and
`bhp_control`'s own corrected bottomhole pressure over the run, against a fixed -20 kg/s target.
That target is negative because it follows [PorousFlowPeacemanBorehole.md]'s own `outflow` sign
convention, already used throughout this example by `character`/`cased_character`: production
*removes* mass from the porespace, so `outflow > 0`; injection *adds* mass, so `outflow < 0`.
A negative injection rate is this well adding mass to the formation, not withdrawing it.
`bhp_control` starts at `initial_pressure`, calibrated ahead of time (via a short fixed-pressure
sweep of this same model) to deliver close to the target rate from the very first timestep,
rather than an arbitrary guess the controller would otherwise spend a long time correcting away
from. The delivered rate settles to within about 2 kg/s of target within roughly a month of
simulated time, then tracks it closely for the remainder of the 5-year run as `bhp_control`
drifts gradually upward from about 22 to 29 MPa - the near-well reservoir pressure itself rising
as injected mass accumulates, eroding some of the available underpressure at a fixed
`bhp_control`. Total injected mass over the run is about 3.11 billion kg, only about 1.6% below
target*(elapsed time) - almost entirely from the initial few weeks' ramp-up, not any ongoing
steady-state error.

!media geothermal_wellbore_injection_rate_and_bhp.png
  id=geothermal_wellbore_injection_rate_fig
  style=width:75%;margin-left:auto;margin-right:auto;
  caption=Delivered injection rate (left axis) and controlled bottomhole pressure (right axis)
  vs. time.

[geothermal_wellbore_injection_flux_fig] shows why `injection_zones.i`'s high-permeability
sub-zone exists: per-point injected mass flux vs. depth, at the end of the run. The three points
inside the sub-zone (y=-1400 to -1600, 10x the surrounding reservoir's permeability) carry about
8.6x the flux of their uniform-permeability neighbors - close to, but not exactly, the
permeability ratio itself, since the higher-flow zone also locally relieves its own overpressure
somewhat faster than its neighbors. Nothing in this model explicitly computes or imposes that
split: it falls directly out of [PorousFlowPeacemanBorehole.md]'s own well-index formula, which
already scales with local permeability, once
[PorousFlowRateControlledBoreholePressure.md] has picked a single bottomhole pressure for the
whole well.

!media geothermal_wellbore_injection_flux_vs_depth.png
  id=geothermal_wellbore_injection_flux_fig
  style=width:55%;margin-left:auto;margin-right:auto;
  caption=Injected mass flux vs. depth at each open-interval point, final step. Shaded band: the
  high-permeability sub-zone.

The cased-section conductive exchange totals only about 0.7% of the advected heat rate here -
smaller, relative to its own well's advected total, than the production well's own ~9% figure
above. `injection_temperature` is fixed and well below the formation temperature throughout, so
(unlike production's mass-flux-weighted mixing temperature, which itself evolves with the
formation) the driving temperature difference here does not grow over time the same way,
capping how much this term can contribute over a 5-year run.


