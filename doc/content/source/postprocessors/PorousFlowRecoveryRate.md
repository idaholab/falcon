# PorousFlowRecoveryRate

!syntax description /Postprocessors/PorousFlowRecoveryRate

## Description

`PorousFlowRecoveryRate` computes the thermal recovery rate (as a percentage) of a doublet
system, defined as the ratio of extracted to injected energy. On every timestep, the sum of the
`hotwellenergy` and `coldwellenergy` postprocessors is added to an injection accumulator if
`hotwellenergy` is less than or equal to zero, or to an extraction accumulator otherwise (the
sign of `hotwellenergy` is used as a proxy for whether the well is currently injecting or
producing). The reported value is `0` until energy has been accumulated on the *injection* side,
and thereafter `abs(extraction)/abs(injection)*100`. The guard is on the denominator, so a run
that begins with production reports `0` rather than dividing by zero.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_recovery_rate/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowRecoveryRate

!syntax inputs /Postprocessors/PorousFlowRecoveryRate

!syntax children /Postprocessors/PorousFlowRecoveryRate
