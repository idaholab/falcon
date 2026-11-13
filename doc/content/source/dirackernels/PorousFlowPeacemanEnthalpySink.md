# PorousFlowPeacemanEnthalpySink

!syntax description /DiracKernels/PorousFlowPeacemanEnthalpySink

## Overview

[PorousFlowPeacemanBorehole.md]'s own `use_enthalpy = true` mode multiplies its mass outflow by
the *local solved field's* own enthalpy - correct for a production well, where the fluid leaving
the formation genuinely carries the formation's own enthalpy, but wrong for an injection well,
where the fluid entering carries the *injected* fluid's enthalpy instead.

`PorousFlowPeacemanEnthalpySink` is the injection-side counterpart: it inherits
`PorousFlowPeacemanBorehole` directly, and overrides `computeQpBaseOutflow()`/
`computeQpBaseOutflowJacobian()` (the same pure-virtual-in-`PorousFlowLineSink`,
overridable-but-not-`final`-in-`PorousFlowPeacemanBorehole` hook that class itself uses) to
multiply the inherited Peaceman well-index-based mass outflow by

\begin{equation}
h = h_{\mathrm{fluid}}(p, T_{\mathrm{inj}}) \ ,
\end{equation}

the enthalpy of fluid at the local reservoir pressure $p$ and a prescribed
`injection_temperature`, rather than at the local solved temperature. This exactly mirrors
FALCON's own [EnthalpySink.md] (which applies the same "multiply by a fixed inlet temperature's
enthalpy" idea on top of `PorousFlowPolyLineSink`'s prescribed-flux mechanism) - the difference
is purely which parent's mass flux gets multiplied: this class needs the mass side to be
Peaceman-driven, so that it can be rate-controlled (see
[PorousFlowRateControlledBoreholePressure.md]) and so that its permeability-based partitioning
across points carries over unchanged.

Two constructor guards catch the two ways this could be silently misconfigured:
`use_enthalpy = true` would double-apply enthalpy (once via this class's own multiplication, once
via the inherited `use_enthalpy` path), and `function_of != pressure` would be inconsistent with
a mass flux driven by a pressure difference.

## Known simplifications

- `injection_temperature` is the raw temperature the fluid has when it reaches this well's own
  points - it is not fed from any upstream conductive warming/cooling (eg
  [PorousFlowCasedBoreholeHeatExchange.md]'s own computed temperature at the open/cased
  boundary, for a well that also has a cased section above). Documented as a known
  simplification in the [geothermal_wellbore.md] injection example, in the same spirit as that
  example's own note that its conductive term is secondary (~9%) next to the advective one.
- Like `PorousFlowLineSink`'s own `use_mobility` option, this multiplies by the *reservoir-side*
  (not upstream/injected-fluid) mobility - for a cold injectant into a hot reservoir, the
  injected fluid's own viscosity is materially higher, so the mass rate at a given overpressure
  is somewhat overstated. This is an existing PorousFlow modelling choice, not something this
  class introduces.

## Example Input File Syntax

!listing examples/geothermal_wellbore/wells_injection.i block=DiracKernels/inject_heat

!syntax parameters /DiracKernels/PorousFlowPeacemanEnthalpySink

!syntax inputs /DiracKernels/PorousFlowPeacemanEnthalpySink

!syntax children /DiracKernels/PorousFlowPeacemanEnthalpySink
