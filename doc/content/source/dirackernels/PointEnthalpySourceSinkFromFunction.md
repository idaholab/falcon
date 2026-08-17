# PointEnthalpySourceSinkFromFunction

!syntax description /DiracKernels/PointEnthalpySourceSinkFromFunction

## Description

`PointEnthalpySourceSinkFromFunction` adds a point contribution to the energy (temperature)
equation representing the heat carried by fluid that is either injected into, or extracted from,
a single point, with the mass flux and (for injection) the inlet temperature given by two
user-supplied functions of time: `mass_flux_function` and `temperature_function`.

At every residual evaluation the object inspects the signs of `mass_flux_function(t)` and
`temperature_function(t)` to decide which of the two physical regimes applies:

- extraction ($\dot{m} \geq 0$ and $T_{in} \leq 0$): the enthalpy is evaluated at the LOCAL
  solution temperature, i.e. fluid leaves at the current reservoir temperature, as for
  [PointEnthalpySink](PointEnthalpySink.md);
- injection ($\dot{m} \leq 0$ and $T_{in} > 0$): the enthalpy is evaluated at the function-supplied
  temperature $T_{in}(t)$, as for
  [PointEnthalpySourceFromFunction](PointEnthalpySourceFromFunction.md).

Any other combination of signs (i.e. the two functions do not agree on whether the point is
injecting or extracting at a given time) is considered a user input error, and the object calls
`mooseError` reporting that "the functions of mass flux and temperature are not coordinated for
injection and extraction". This is by design: the convention `temperature_function <= 0` during
extraction is how the object is told "there is no prescribed temperature; use the local value
instead", so `mass_flux_function` and `temperature_function` must always be set up consistently
by the user for a single point that switches between injection and production over time (e.g. for
a push-pull or seasonal thermal storage cycle).

The residual contribution,

\begin{equation}
\dot{Q} = \dot{m}(t) \, h,
\end{equation}

is accumulated (multiplied by the timestep size) into the `SumQuantityUO`
[PorousFlowSumQuantity](PorousFlowSumQuantity.md) user object every timestep, so that the total
energy injected/extracted can be recovered with a
[PorousFlowPlotQuantity](PorousFlowPlotQuantity.md) postprocessor.

## Example Input Syntax

!listing test/tests/dirackernels/point_enthalpy_source_sink_from_function/inp.i block=DiracKernels

!syntax parameters /DiracKernels/PointEnthalpySourceSinkFromFunction

!syntax inputs /DiracKernels/PointEnthalpySourceSinkFromFunction

!syntax children /DiracKernels/PointEnthalpySourceSinkFromFunction
