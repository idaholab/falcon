# PorousFlowDoubletBreakthroughTime

!syntax description /Postprocessors/PorousFlowDoubletBreakthroughTime

## Description

`PorousFlowDoubletBreakthroughTime` records the simulation time, in days, at which a doublet
breakthrough indicator postprocessor (typically a
[PorousFlowDoubletBreakthroughTerminator.md]) first reaches `0.5` or above. The recorded value
starts at `0` and latches, on the first timestep the indicator crosses the threshold, to the
current simulation time (`timepostprocessor`, in seconds) converted to days; it is not
re-latched by any subsequent crossing.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_doublet_breakthrough_time/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowDoubletBreakthroughTime

!syntax inputs /Postprocessors/PorousFlowDoubletBreakthroughTime

!syntax children /Postprocessors/PorousFlowDoubletBreakthroughTime
