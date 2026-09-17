# PorousFlowRecoveryTime

!syntax description /Postprocessors/PorousFlowRecoveryTime

## Description

`PorousFlowRecoveryTime` computes the elapsed time, in days, since a doublet breakthrough time
recorded by another postprocessor (typically a [PorousFlowDoubletBreakthroughTime.md]). It is a
purely algebraic postprocessor: once a breakthrough time has been recorded, the reported value is
`timepostprocessor/3600/24 - breakthroughtime`, i.e. the current simulation time converted to
days, minus the recorded breakthrough time (also in days), which increases linearly with
simulation time. Before breakthrough is recorded, [PorousFlowDoubletBreakthroughTime.md] reports
`0`; taken literally that would make this object report the whole elapsed simulation time as
though recovery had already begun, so it instead reports `0` until a positive breakthrough time
is available.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_recovery_time/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowRecoveryTime

!syntax inputs /Postprocessors/PorousFlowRecoveryTime

!syntax children /Postprocessors/PorousFlowRecoveryTime
