# PorousFlowRecoveryRateSeason

!syntax description /Postprocessors/PorousFlowRecoveryRateSeason

## Description

`PorousFlowRecoveryRateSeason` computes the seasonal thermal recovery rate (as a percentage) of
a doublet system, analogous to [PorousFlowRecoveryRate.md] but gated by explicit season indicator
postprocessors rather than the sign of the hot-well energy. On every timestep, the sum of the
`hotwellenergy` and `coldwellenergy` postprocessors is added to an injection accumulator if
`InjectionIndicator` is exactly `1`, or to a production accumulator if `ProductionIndicator` is
exactly `1` (checked only when `InjectionIndicator` is not `1`). The reported value is `0` until
any production has occurred, and thereafter `abs(production)/abs(injection)*100`.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_recovery_rate_season/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowRecoveryRateSeason

!syntax inputs /Postprocessors/PorousFlowRecoveryRateSeason

!syntax children /Postprocessors/PorousFlowRecoveryRateSeason
