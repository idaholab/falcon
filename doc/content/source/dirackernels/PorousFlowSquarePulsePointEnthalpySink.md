# PorousFlowSquarePulsePointEnthalpySink

!syntax description /DiracKernels/PorousFlowSquarePulsePointEnthalpySink

## Description

`PorousFlowSquarePulsePointEnthalpySink` adds a point contribution to the energy (temperature)
equation representing the heat carried away by fluid extracted at a single point, at a constant
mass flux `mass_flux`, but only during the time window $[$`start_time`$,$ `end_time`$]$
("square pulse"). As for [PointEnthalpySink](PointEnthalpySink.md), the enthalpy is evaluated at
the LOCAL solution temperature (not a fixed or prescribed value), so this object represents
extraction at the current reservoir temperature:

\begin{equation}
\dot{Q} = \mathrm{factor}(t) \, \dot{m} \, h(P, T)
\end{equation}

where $\mathrm{factor}(t) \in [0, 1]$ ramps the mass flux linearly on and off across whichever
timestep(s) straddle `start_time` or `end_time`, so that the total mass (and hence energy) removed
over the pulse is exact regardless of the timestep size used. Denoting the current timestep
$(t-\Delta t, t]$:

- $\mathrm{factor} = 0$ if the pulse has not yet started ($t <$ `start_time`) or has already ended
  ($t-\Delta t \geq$ `end_time`);
- $\mathrm{factor} = 1$ if the pulse is active for the whole timestep;
- otherwise $\mathrm{factor}$ is the fraction of $\Delta t$ during which the pulse was active,
  handling the case where `start_time` and/or `end_time` fall strictly inside a single timestep.

The constructor errors if `end_time` is not strictly greater than `start_time`.

## Example Input Syntax

!listing test/tests/dirackernels/square_pulse_point_enthalpy_sink/inp.i block=DiracKernels

!syntax parameters /DiracKernels/PorousFlowSquarePulsePointEnthalpySink

!syntax inputs /DiracKernels/PorousFlowSquarePulsePointEnthalpySink

!syntax children /DiracKernels/PorousFlowSquarePulsePointEnthalpySink
