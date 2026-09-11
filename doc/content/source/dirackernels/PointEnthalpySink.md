# PointEnthalpySink

!syntax description /DiracKernels/PointEnthalpySink

## Description

`PointEnthalpySink` adds a point contribution to the energy (temperature) equation representing
the heat carried away by fluid extracted at a single point, at a mass flux rate given by the
`mass_flux` postprocessor:

\begin{equation}
\dot{Q} = \dot{m} \, h(P, T)
\end{equation}

where $\dot{m}$ is the (postprocessor-supplied) mass flux and $h$ is the fluid enthalpy evaluated
by the `fp` fluid properties user object at the local porepressure and the LOCAL solution
temperature (the `PorousFlow_temperature_qp` material property), not a fixed or prescribed value.
This makes `PointEnthalpySink` appropriate for representing production at the local reservoir
temperature: as the reservoir cools (or heats), the enthalpy of the extracted fluid tracks the
current local temperature.

`PointEnthalpySink` should be applied to the temperature variable, typically alongside a
companion point mass sink (e.g. `PorousFlowSquarePulsePointSource` or a reporter-driven point
sink) acting on the pressure variable, driven by the same `mass_flux` postprocessor, so that the
mass and energy removed are consistent.

## Example Input Syntax

!listing test/tests/dirackernels/point_enthalpy_sink/inp.i block=DiracKernels

!syntax parameters /DiracKernels/PointEnthalpySink

!syntax inputs /DiracKernels/PointEnthalpySink

!syntax children /DiracKernels/PointEnthalpySink
