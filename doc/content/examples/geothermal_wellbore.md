# Geothermal well production and wellbore-reservoir heat exchange

This example demonstrates a single geothermal production well: mass extraction and
wellbore-reservoir heat exchange using [PorousFlowPeacemanBorehole.md]'s temperature-dependent
wellbore-pressure option (`unit_weight_fp`), together with per-point flux reporting via
[PorousFlowPointFluxQuantity.md]/[PorousFlowPlotPointFluxQuantity.md].

## Problem Description

A single production well sits at the center of an axisymmetric (RZ) reservoir: a 1000m-radius,
2000m-deep domain with a 30 degC surface temperature and a 100 degC/km geothermal gradient. The
top 1000m is a lower-permeability cap; the well is cased through the cap and open to the
formation only over its bottom 1000m (403 K to 503 K) - so casing coincides with the cap and the
open completion with the reservoir, matching conventional well design.

Two [PorousFlowPeacemanBorehole.md] DiracKernels share the same well geometry: one on
`porepressure` extracts fluid at a fixed bottomhole pressure; a second on `temperature`, with
`use_enthalpy = true`, extracts heat from the same points. This is the correct (and only)
mechanism for well-to-formation heat exchange in the PorousFlow module - it is advective only,
via the enthalpy carried by the extracted fluid; there is no separate conductive
well-to-formation heat-transfer term.

The reservoir's own permeability and the cap's thermal conductivity/specific heat are set well
outside typical geologic ranges. This is a deliberate idealization for clarity, not a claim
about real rock properties - with realistic values, the well's thermal interaction with the
surrounding formation would either spread across nearly the entire domain (a highly permeable
reservoir lets the well's pressure influence diffuse over kilometers within a few years) or
take millennia to become visible at all (real caprock's thermal diffusivity is far too low to
show any signal on a human timescale). The values used here keep the well's influence
confined to a visible halo of a few hundred meters, on a runnable 5-year timescale - the same
kind of idealization moose's own [ates.md] example makes for its aquifer thermal energy storage
demonstration.

## Implementation Overview

- [PorousFlowPeacemanBorehole.md] - the two wells (mass extraction and heat exchange), sharing
  one `point_file` and `character` function so the open interval is identical for both.
- [PorousFlowPointFluxQuantity.md] / [PorousFlowPlotPointFluxQuantity.md] - report the flow-rate
  *profile* along the well (one value per point), rather than only the well-wide total from
  [PorousFlowSumQuantity.md]/[PorousFlowPlotQuantity.md].
- `unit_weight_fp` (see [PorousFlowPeacemanBorehole.md] and the
  [porous_flow/sinks.md#the-wellbore-pressure-with-a-temperature-dependent-fluid-density]
  section it links to) builds the wellbore pressure profile from the fluid density at the local
  temperature at each well point, instead of a single constant `unit_weight` - important here
  because the well's open interval spans a 100 K range (403-503 K).

`model_common.i` holds everything except the well (mesh, materials, geothermal field, fluid
properties, solver); `production.i` and `production_constant_unit_weight.i` each
`!include` it plus a different well-definition file (`wells_variable_unit_weight.i` /
`wells_constant_unit_weight.i`), so the two are identical everywhere except the well's
wellbore-pressure treatment.

## Running the Example

```
falcon-opt -i production.i                          # the new unit_weight_fp behaviour
falcon-opt -i production_constant_unit_weight.i      # the old constant-unit_weight behaviour
python3 plot_results.py                              # produces the two plots below
```

## Results

[production_rates] shows the extracted mass and heat rate over the 5-year run, for both
wellbore-pressure treatments.

!media geothermal_wellbore_rates_vs_time.png
  id=production_rates
  style=width:70%;margin-left:auto;margin-right:auto;
  caption=Extracted mass (top) and heat (bottom) per timestep, old (constant `unit_weight`)
  vs. new (`unit_weight_fp`).

Over the full run, the old constant-`unit_weight` treatment extracts about 17% more mass and
heat than the new temperature-dependent treatment - a substantial difference, driven by the
100 K temperature range spanned by this well's open interval. The old treatment's constant was
itself chosen as a reasonable single-point estimate (density at the open interval's mean depth
conditions), illustrating the practical benefit of `unit_weight_fp`: it removes the need to
hand-derive and re-check that constant every time a well's design changes.

[production_halo] shows the temperature at the cap/reservoir interface (right where the well's
open interval begins) as a function of radius, at the end of the run - a real, well-centered
thermal halo around the well, roughly 100-300m across.

!media geothermal_wellbore_temperature_halo.png
  id=production_halo
  style=width:60%;margin-left:auto;margin-right:auto;
  caption=Temperature vs. radius at the cap/reservoir interface, final timestep.
