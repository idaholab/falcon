# Geothermal well production and wellbore-reservoir heat exchange

## Introduction

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
[PorousFlowPlotPointFluxQuantity.md]. It models a single well: mass extraction, plus
wellbore-reservoir heat exchange (a second [PorousFlowPeacemanBorehole.md] with
`use_enthalpy = true`) - the correct, and only, mechanism for well-to-formation heat exchange in
the PorousFlow module, since it is advective only, via the enthalpy carried by the extracted
fluid.

## Model setup

The well has radial symmetry, so it is simulated using RZ coordinates - the same approach used
by moose's own [ates.md] example:

!listing examples/geothermal_wellbore/model_common.i block=Mesh

which means gravity acts along what is usually thought of as the "y" direction:

!listing examples/geothermal_wellbore/model_common.i block=GlobalParams

The geometry is shown in [geothermal_wellbore_geometry_fig]: a 1000m-radius, 2000m-deep domain,
with a lower-permeability cap over the top 1000m and the reservoir below it. The well is cased
through the cap and open to the formation only over its bottom 1000m - so casing coincides with
the cap and the open completion with the reservoir, matching conventional well design.

!media geothermal_wellbore_schematic.png
  id=geothermal_wellbore_geometry_fig
  style=width:80%;margin-left:auto;margin-right:auto;
  caption=Geometry on a radial slice of the geothermal wellbore model.

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
