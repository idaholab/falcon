# FunctionEnthalpySink

!syntax description /DiracKernels/FunctionEnthalpySink

## Description

`FunctionEnthalpySink` is identical to [EnthalpySink](EnthalpySink.md), except that the inlet
temperature used to evaluate the fluid enthalpy is given by a `function` of time and position
rather than a fixed constant:

\begin{equation}
\dot{Q} = \dot{m}(P) \, h\left(P, T_{in}(t, \vec{x})\right)
\end{equation}

where $\dot{m}(P)$ is the [PorousFlowPolyLineSink](PorousFlowPolyLineSink.md) mass outflow rate
(a piecewise-linear function of the local porepressure) and $T_{in}(t,\vec{x})$ is evaluated by
the `function` parameter at the current time and the quadrature point location. This allows the
inlet/outlet temperature of a line sink to vary over the course of a simulation, for example to
model a time-varying offtake temperature.

## Example Input Syntax

!listing test/tests/dirackernels/function_enthalpy_sink/inp.i block=DiracKernels

!syntax parameters /DiracKernels/FunctionEnthalpySink

!syntax inputs /DiracKernels/FunctionEnthalpySink

!syntax children /DiracKernels/FunctionEnthalpySink
