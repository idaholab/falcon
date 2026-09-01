# PorousFlowRecoveryTime

!syntax description /Postprocessors/PorousFlowRecoveryTime

## Description

`PorousFlowRecoveryTime` computes the elapsed time, in days, since a doublet breakthrough time
recorded by another postprocessor (typically a [PorousFlowDoubletBreakthroughTime.md]). It is a
purely algebraic postprocessor: the reported value is simply
`timepostprocessor/3600/24 - breakthroughtime`, i.e. the current simulation time converted to
days, minus the recorded breakthrough time (also in days). The result is negative before
breakthrough has been recorded and increases linearly with simulation time thereafter.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_recovery_time/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowRecoveryTime

!syntax inputs /Postprocessors/PorousFlowRecoveryTime

!syntax children /Postprocessors/PorousFlowRecoveryTime
