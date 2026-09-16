# Geothermal well production and wellbore-reservoir heat exchange

## Introduction

This example has two goals: to show how a temperature-dependent fluid density changes the
pressure needed to produce hot water from a geothermal well, and to show the two distinct
mechanisms by which the well exchanges heat with the rock around it - advectively, where fluid
actually crosses into the well, and conductively, through the casing wall above that.

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
  separate numerical limit remains, unrelated to pressurization: above roughly
  $h \sim 1.5\text{-}1.8\ \mathrm{W/m^2/K}$, the simulation stalls around day 156 of the run
  (confirmed by bisection; raising the solver's iteration limit does not rescue it), so 1.5 stays
  just below that wall. The resulting conductive exchange totals about 6.6% of the advected heat
  rate $Q_{\mathrm{adv}}$ over the run - real, but secondary to a much larger effect: at this
  permeability, the cap itself is porous enough for the well's own pressure drawdown to drive
  significant advective flow up through it, which dominates the temperature changes shown in
  [#results] far more than this conductive term does.

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
formation and enters the well.

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

The two wells share one `point_file` and `character` function, so the open interval is identical
for the mass-extraction and heat-exchange DiracKernels:

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=DiracKernels

`production.i` uses this well definition; `production_constant_unit_weight.i` swaps in
`wells_constant_unit_weight.i` instead, which is otherwise identical except for a single
constant `unit_weight` (see that file for how it was derived) in place of `unit_weight_fp` -
everything else in the two input files, down to the shared `model_common.i`, is the same.

## Running the Example

```
falcon-opt -i production.i                          # the new unit_weight_fp behaviour
falcon-opt -i production_constant_unit_weight.i      # the old constant-unit_weight behaviour
python3 plot_results.py                              # produces the plots below
python3 make_schematic.py                             # produces the geometry schematic above
```

## Results

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
  caption=Extracted mass (top) and heat (bottom) per timestep, old (constant `unit_weight`) vs.
  new (`unit_weight_fp`).

[geothermal_wellbore_pressure_fig] shows exactly where that difference comes from: pressure vs.
depth along the well, at the end of the run, for both treatments, plus their difference. The two
profiles diverge smoothly through the open interval, reaching about 153 kPa at its midpoint
before converging back to zero at the well bottom - where `bottom_p_or_t` pins both treatments
to the same value by construction, regardless of the density model used above it. Through the
cased section (y=0 to -1000) the difference no longer sits at zero the way it would in a purely
diffusive cap: it grows smoothly from about 1.7 kPa at the surface to about 65 kPa at the
cap/reservoir interface, because the cap's realistic permeability lets some of the open
interval's pressure-treatment difference propagate up through it via the same advective flow
discussed in [#model-setup] and shown in [geothermal_wellbore_temperature_depths_fig] - not
because any mass crosses the well wall there, which it still does not.

!media geothermal_wellbore_pressure_depth.png
  id=geothermal_wellbore_pressure_fig
  style=width:95%;margin-left:auto;margin-right:auto;
  caption=Left: pressure vs. depth along the well, final step, old vs. new. Right: their
  difference vs. depth.

[geothermal_wellbore_temperature_depths_fig] shows how temperature changes over time at a series
of depths along the well axis, for the `unit_weight_fp` treatment, and the picture is very
different from a purely conductive cap: temperature rises substantially throughout the cased
section, not just near its two ends. At the surface (y=0m) it rises by about 67 K over 5 years;
at y=-200, -400, -600 and -800m the rises are about 58, 50, 42 and 34 K respectively - a smooth
gradient, largest near the surface and decreasing with depth through the cap. This is not the
new conductive heat exchange at work (which totals only about 6.6% of the advected heat rate):
it is the realistically-permeable cap itself allowing the well's pressure drawdown to pull fluid,
and the heat it carries, up through the cased section by advection - a genuinely different, and
much larger, mechanism than the small conductive term derived in [#borehole-heat-exchange]. Right
at the cap/reservoir interface (y=-1000) the change is a much more modest 1.2 K, and below it -
in the open interval and reservoir - the picture reverts to what drives the mass/heat extraction
comparison above: temperature at the well bottom (y=-2000m) falls by about 2.4 K as continuous
heat extraction outpaces replenishment from the surrounding reservoir, while y=-1500m barely
changes at all.

!media geothermal_wellbore_temperature_depths.png
  id=geothermal_wellbore_temperature_depths_fig
  style=width:80%;margin-left:auto;margin-right:auto;
  caption=Temperature change (relative to the start of the run) vs. time, at depths along the
  well axis, `unit_weight_fp` treatment.

[geothermal_wellbore_halo_fig] shows the temperature at the cap/reservoir interface (right where
the well's open interval begins) as a function of radius, at the end of the run - a real,
well-centered thermal halo around the well for both treatments, peaking at about 404.3 K against
a 403.2 K background and extending to about 60m radius.

!media geothermal_wellbore_temperature_halo.png
  id=geothermal_wellbore_halo_fig
  style=width:65%;margin-left:auto;margin-right:auto;
  caption=Temperature vs. radius at the cap/reservoir interface, final timestep.

[geothermal_wellbore_cap_temperature_fig] shows the same kind of radial profile, but strictly
*inside* the cap, 500m below the surface, and at a handful of times through the run, rather than
a single final snapshot at the cap/reservoir boundary. The profile is flat at the undisturbed
background (353.15 K, the geothermal gradient's value at this depth) at early times, then a
sharp, growing spike develops right at the well - reaching about 399 K by the end of the run,
46 K above background, and decaying back to background within about 20m. This is the same
advective mechanism visible in [geothermal_wellbore_temperature_depths_fig], now shown as a
function of radius rather than time: hot fluid drawn up along the well axis through the
realistically-permeable cap, not the new conductive heat exchange kernel, whose own contribution
(reported separately via `wellbore_heat_point_flux_out`, not distinguishable from the advective
signal in a plain temperature snapshot like this one) is an order of magnitude smaller.

!media geothermal_wellbore_cap_temperature_radial.png
  id=geothermal_wellbore_cap_temperature_fig
  style=width:65%;margin-left:auto;margin-right:auto;
  caption=Temperature vs. radius at 500m depth within the cap, at several times through the
  run, the `unit_weight_fp` treatment.


