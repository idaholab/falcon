# PorousFlowEnergyAccumulator

!syntax description /Postprocessors/PorousFlowEnergyAccumulator

## Description

`PorousFlowEnergyAccumulator` accumulates the produced thermal energy of a doublet system. On
every timestep where the supplied `ProductionIndicator` postprocessor is greater than `0.5`, the
sum of the `hotwellenergy` and `coldwellenergy` postprocessors is added to a running total;
timesteps where the indicator is at or below `0.5` (e.g. during an injection-only or idle season)
do not contribute. The comparison is against `0.5` rather than an exact equality test with `1` so
that a smoothed or transfer-round-tripped indicator still registers.
The postprocessor value reported is the absolute value of the running total, so it grows
monotonically over a simulation dominated by production.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_energy_accumulator/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowEnergyAccumulator

!syntax inputs /Postprocessors/PorousFlowEnergyAccumulator

!syntax children /Postprocessors/PorousFlowEnergyAccumulator
