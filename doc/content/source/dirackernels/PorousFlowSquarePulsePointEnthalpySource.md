# PorousFlowSquarePulsePointEnthalpySource

!syntax description /DiracKernels/PorousFlowSquarePulsePointEnthalpySource

## Description

`PorousFlowSquarePulsePointEnthalpySource` adds a point contribution to the energy (temperature)
equation representing the heat carried by fluid injected at a single point, at a constant mass
flux `mass_flux` and a fixed inlet temperature `T_in`, but only during the time window
$[$`start_time`$,$ `end_time`$]$ ("square pulse"):

\begin{equation}
\dot{Q} = \mathrm{factor}(t) \, \dot{m} \, h(P, T_{in})
\end{equation}

where $h$ is evaluated by the `fp` fluid properties user object at the local porepressure and the
fixed `T_in`, and $\mathrm{factor}(t) \in [0, 1]$ ramps the mass flux linearly on and off across
whichever timestep(s) straddle `start_time` or `end_time`, so that the total mass (and hence
energy) injected over the pulse is exact regardless of the timestep size used -- see
[PorousFlowSquarePulsePointEnthalpySink](PorousFlowSquarePulsePointEnthalpySink.md) for the full
enumeration of the ramp cases. Unlike that sink counterpart, this object always uses the fixed
`T_in` rather than the local solution temperature, and its residual carries the opposite sign
convention (it adds, rather than removes, heat energy for a positive `mass_flux`).

The constructor errors if `end_time` is not strictly greater than `start_time`.

## Example Input Syntax

!listing test/tests/dirackernels/square_pulse_point_enthalpy_source/inp.i block=DiracKernels

!syntax parameters /DiracKernels/PorousFlowSquarePulsePointEnthalpySource

!syntax inputs /DiracKernels/PorousFlowSquarePulsePointEnthalpySource

!syntax children /DiracKernels/PorousFlowSquarePulsePointEnthalpySource
