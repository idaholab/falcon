# PorousFlowDoubletBreakthroughTerminator

!syntax description /Postprocessors/PorousFlowDoubletBreakthroughTerminator

## Description

`PorousFlowDoubletBreakthroughTerminator` reports a `0`/`1` indicator of whether thermal
breakthrough has been detected at a doublet's production well. On every timestep, the absolute
difference between a target postprocessor (typically the produced fluid temperature) and
`temperature_init` is compared against `temperature_tolerance`; the reported value is `1` once
that difference is at or above the tolerance, and `0` otherwise (note that, unlike the other
`*Terminator` postprocessors in FALCON, this indicator is not latched: it can revert to `0` again
if the target postprocessor's value moves back within tolerance of `temperature_init`).

This postprocessor only *reports* the indicator; it does not stop the simulation by itself. To
actually terminate the run once breakthrough is detected, pair it with a
[Terminator](Terminator.md) `[UserObjects]` block whose `expression` tests this postprocessor's
value, as shown in the example below.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_doublet_breakthrough_terminator/inp.i block=Postprocessors

!listing test/tests/postprocessors/porous_flow_doublet_breakthrough_terminator/inp.i block=UserObjects

!syntax parameters /Postprocessors/PorousFlowDoubletBreakthroughTerminator

!syntax inputs /Postprocessors/PorousFlowDoubletBreakthroughTerminator

!syntax children /Postprocessors/PorousFlowDoubletBreakthroughTerminator
