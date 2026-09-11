# EnthalpySink

!syntax description /DiracKernels/EnthalpySink

## Description

`EnthalpySink` is a [PorousFlowPolyLineSink](PorousFlowPolyLineSink.md) that represents the
energy carried by fluid extracted through a polyline (well) sink. It reuses the base class'
pressure-dependent mass outflow rate (a piecewise-linear function of the local porepressure, see
`p_or_t_vals`/`fluxes`), and multiplies that mass outflow by the specific enthalpy of the fluid,
evaluated at the local porepressure and a fixed inlet temperature `T_in`:

\begin{equation}
\dot{Q} = \dot{m}(P) \, h(P, T_{in})
\end{equation}

where $\dot{m}(P)$ is the base class' mass outflow rate and $h$ is computed by the `fp` fluid
properties user object. `EnthalpySink` should be applied to the energy (temperature) equation,
alongside a companion mass sink (typically a plain `PorousFlowPolyLineSink` acting on the pressure
variable) so that the mass and energy removed from the model are consistent.

Because the outflow is multiplied by a fixed `T_in` rather than the local temperature, this object
is most appropriate for a line sink where the fluid leaves the reservoir at a known/controlled
temperature (e.g. a fixed-temperature offtake), as opposed to production at the local reservoir
temperature (for which [PointEnthalpySink](PointEnthalpySink.md) or
[PorousFlowSquarePulsePointEnthalpySink](PorousFlowSquarePulsePointEnthalpySink.md) may be more
appropriate).

## Example Input Syntax

!listing test/tests/dirackernels/enthalpy_sink/inp.i block=DiracKernels

!syntax parameters /DiracKernels/EnthalpySink

!syntax inputs /DiracKernels/EnthalpySink

!syntax children /DiracKernels/EnthalpySink
