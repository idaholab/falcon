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
  formation's, which is already resolved by the mesh, and is deliberately kept small
  (0.01 $\mathrm{W/m^2/K}$, well below a nominal casing-resistance estimate) for a reason
  specific to this example: the cap's permeability is set very low precisely so the well's
  pressure influence stays confined to a small halo, and heat injected into a low-permeability
  rock thermally pressurizes the trapped pore fluid instead of relieving via flow. Above roughly
  0.1 $\mathrm{W/m^2/K}$ that pressurization runs away and the simulation fails to converge,
  regardless of how small a fraction of the advected heat rate $Q_{\mathrm{adv}}$ it represents.
  The resulting conductive exchange is real but small - about 0.08% of the advected heat rate -
  a real effect, not a dominant one; see [geothermal_wellbore_cap_temperature_fig] for where it
  shows up.

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

The cap's permeability and thermal properties are deliberately set well outside typical geologic
ranges, and the reservoir's permeability is likewise lowered - a deliberate idealization for
clarity, not a claim about real rock properties. With realistic values, the well's influence
would either spread across nearly the entire domain (a highly permeable reservoir lets pressure
diffuse over kilometers within a few years) or take millennia to become visible at all (real
caprock's thermal diffusivity is far too low to show any signal on a human timescale). The values
below keep the well's influence confined to a visible halo a few hundred meters across, on a
runnable 5-year timescale:

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
profiles agree almost exactly through the cased section (y=0 to -1000, no flow enters there
either way), then diverge smoothly through the open interval, reaching about 150 kPa at its
midpoint before converging back to zero at the well bottom - where `bottom_p_or_t` pins both
treatments to the same value by construction, regardless of the density model used above it.

!media geothermal_wellbore_pressure_depth.png
  id=geothermal_wellbore_pressure_fig
  style=width:95%;margin-left:auto;margin-right:auto;
  caption=Left: pressure vs. depth along the well, final step, old vs. new. Right: their
  difference vs. depth.

[geothermal_wellbore_temperature_depths_fig] shows how temperature changes over time at a series
of depths along the well axis, for the `unit_weight_fp` treatment. At the surface (y=0m, in the
cap, directly above the well but outside its cased section) temperature rises by more than 6 K
over 5 years, as the cap's exaggerated thermal diffusivity carries heat up from the open interval
below. At the well bottom (y=-2000m, in the open interval) temperature falls by about 2.5 K, as
continuous heat extraction outpaces replenishment from the surrounding reservoir. The
intermediate depths (y=-200 to -1500m) change by well under 0.5 K over the same period - the
thermal signal is concentrated at the two ends of the column, not spread evenly along it.

!media geothermal_wellbore_temperature_depths.png
  id=geothermal_wellbore_temperature_depths_fig
  style=width:80%;margin-left:auto;margin-right:auto;
  caption=Temperature change (relative to the start of the run) vs. time, at depths along the
  well axis, `unit_weight_fp` treatment.

[geothermal_wellbore_halo_fig] shows the temperature at the cap/reservoir interface (right where
the well's open interval begins) as a function of radius, at the end of the run - a real,
well-centered thermal halo around the well, roughly 100-300m across, for both treatments.

!media geothermal_wellbore_temperature_halo.png
  id=geothermal_wellbore_halo_fig
  style=width:65%;margin-left:auto;margin-right:auto;
  caption=Temperature vs. radius at the cap/reservoir interface, final timestep.

[geothermal_wellbore_cap_temperature_fig] shows the same kind of radial profile, but strictly
*inside* the cap (y=-500, `unit_weight_fp` treatment only) and at a handful of times through the
run, rather than a single final snapshot at the cap/reservoir boundary. The profile is flat at
the undisturbed background (353.15 K, the geothermal gradient's value at this depth) at early
times, then a small, growing elevation appears within about 50m of the well as the cased
section's conductive heat exchange (and whatever advective signal has diffused up from the open
interval below) warms the near-well cap over the 5-year run - visibly real, but consistent with
[#borehole-heat-exchange]'s ~0.08% figure: a few hundredths of a kelvin, not the multi-kelvin
halo already visible at the cap/reservoir interface in [geothermal_wellbore_halo_fig].

!media geothermal_wellbore_cap_temperature_radial.png
  id=geothermal_wellbore_cap_temperature_fig
  style=width:65%;margin-left:auto;margin-right:auto;
  caption=Temperature vs. radius at mid-cap depth (y=-500), at several times through the run,
  `unit_weight_fp` treatment.
