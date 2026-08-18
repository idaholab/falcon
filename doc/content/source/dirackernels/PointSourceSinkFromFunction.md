# PointSourceSinkFromFunction

!syntax description /DiracKernels/PointSourceSinkFromFunction

## Description

`PointSourceSinkFromFunction` is a pure mass source/sink: it adds a point contribution to a
PorousFlow mass-balance equation (typically the porepressure variable) at a rate given by the
`mass_flux_function` function of time and position:

\begin{equation}
\dot{m} = \dot{m}(t, \vec{x})
\end{equation}

A positive `mass_flux_function` value is a source of fluid mass into the model, and a negative
value is a sink (extraction). Unlike [PorousFlowSquarePulsePointSource](PorousFlowSquarePulsePointSource.md)
(a constant rate active between `start_time` and `end_time`), the rate here can vary arbitrarily
over time, which is convenient for representing, for example, a time-dependent well schedule read
from a table.

The rate is accumulated (multiplied by the timestep size) into the `SumQuantityUO`
[PorousFlowSumQuantity](PorousFlowSumQuantity.md) user object every timestep, so that the total
mass injected/extracted can be recovered with a
[PorousFlowPlotQuantity](PorousFlowPlotQuantity.md) postprocessor. This object has no associated
Jacobian contribution (the mass flux function does not depend on the solution), so it should not
be expected to improve nonlinear convergence relative to a plain forcing term.

## Example Input Syntax

!listing test/tests/dirackernels/point_source_sink_from_function/inp.i block=DiracKernels

!syntax parameters /DiracKernels/PointSourceSinkFromFunction

!syntax inputs /DiracKernels/PointSourceSinkFromFunction

!syntax children /DiracKernels/PointSourceSinkFromFunction
