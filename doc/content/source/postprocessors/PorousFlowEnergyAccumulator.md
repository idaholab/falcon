# PorousFlowEnergyAccumulator

!syntax description /Postprocessors/PorousFlowEnergyAccumulator

## Description

`PorousFlowEnergyAccumulator` accumulates the produced thermal energy of a doublet system. On
every timestep where the supplied `ProductionIndicator` postprocessor is exactly `1`, the sum of
the `hotwellenergy` and `coldwellenergy` postprocessors is added to a running total; timesteps
where the indicator is not `1` (e.g. during an injection-only or idle season) do not contribute.
The postprocessor value reported is the absolute value of the running total, so it grows
monotonically over a simulation dominated by production.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_energy_accumulator/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowEnergyAccumulator

!syntax inputs /Postprocessors/PorousFlowEnergyAccumulator

!syntax children /Postprocessors/PorousFlowEnergyAccumulator
