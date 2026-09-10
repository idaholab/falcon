# PorousFlowRecoveryRateSeason

!syntax description /Postprocessors/PorousFlowRecoveryRateSeason

## Description

`PorousFlowRecoveryRateSeason` computes the seasonal thermal recovery rate (as a percentage) of
a doublet system, analogous to [PorousFlowRecoveryRate.md] but gated by explicit season indicator
postprocessors rather than the sign of the hot-well energy. On every timestep, the sum of the
`hotwellenergy` and `coldwellenergy` postprocessors is added to an injection accumulator if
`InjectionIndicator` is greater than `0.5`, or to a production accumulator if
`ProductionIndicator` is greater than `0.5` (checked only when `InjectionIndicator` is not). The
indicators are compared against `0.5` rather than tested for exact equality with `1` so that a
smoothed or transfer-round-tripped indicator still registers. The reported value is `0` until
energy has been accumulated on the *injection* side, and thereafter
`abs(production)/abs(injection)*100` -- the guard is on the denominator.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_recovery_rate_season/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowRecoveryRateSeason

!syntax inputs /Postprocessors/PorousFlowRecoveryRateSeason

!syntax children /Postprocessors/PorousFlowRecoveryRateSeason
