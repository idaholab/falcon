# PointEnthalpySourceFromFunction

!syntax description /DiracKernels/PointEnthalpySourceFromFunction

## Description

`PointEnthalpySourceFromFunction` adds a point contribution to the energy (temperature) equation
representing the heat carried by fluid crossing a single point, at a mass flux rate given by the
`mass_flux` postprocessor and a temperature given by a `function` of time and position:

\begin{equation}
R = \dot{m} \, h\left(P, T(t, \vec{x})\right)
\end{equation}

where $\dot{m}$ is the (postprocessor-supplied) mass flux and $h$ is the fluid enthalpy evaluated
by the `fp` fluid properties user object at the local porepressure and the temperature given by
`function`. The contribution enters the residual with a positive sign, so a positive `mass_flux`
*removes* energy from the system (extraction) and a negative `mass_flux` adds it (injection) --
which is why the object is registered as a point sink.

Unlike [PointEnthalpySink](PointEnthalpySink.md), the temperature used to evaluate the enthalpy
does not depend on the local solution: it is a prescribed function of time (and position), which
makes this object convenient wherever the temperature of the stream follows a known schedule
rather than the reservoir state (e.g. a seasonally-varying prescribed temperature).

`PointEnthalpySourceFromFunction` should be applied to the temperature variable, typically
alongside a companion point mass source/sink acting on the pressure variable, driven by the same
`mass_flux` postprocessor.

## Example Input Syntax

!listing test/tests/dirackernels/point_enthalpy_source_from_function/inp.i block=DiracKernels

!syntax parameters /DiracKernels/PointEnthalpySourceFromFunction

!syntax inputs /DiracKernels/PointEnthalpySourceFromFunction

!syntax children /DiracKernels/PointEnthalpySourceFromFunction
