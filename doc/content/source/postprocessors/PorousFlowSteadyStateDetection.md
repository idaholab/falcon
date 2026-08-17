# PorousFlowSteadyStateDetection

!syntax description /Postprocessors/PorousFlowSteadyStateDetection

## Description

`PorousFlowSteadyStateDetection` computes the relative rate of change, between the current and
previous timestep, of a target postprocessor's value normalized by the timestep size. Denoting
the current and old values of `targetpostprocessor` as `value`/`value_old` and of
`timepostprocessor` (the timestep-size postprocessor) as `dt`/`dt_old`, the reported value is

```
abs( (value/dt - value_old/dt_old) / (value_old/dt_old) )
```

except on the very first (`t_step == 0`) evaluation, where it is forced to `0`. This quantity
trends toward zero as a simulation approaches a steady state, and is typically fed into
[PorousFlowSteadyStateTerminator.md] to stop a transient once the system has settled.

Because `value_old` and `dt_old` are only meaningful once a postprocessor has been through at
least one real timestep transition, feeding this postprocessor a `timepostprocessor` whose value
is not yet defined at the simulation's initial time (e.g. `TimestepSize`, whose value before the
executioner has set a timestep is `0`) will produce a division-by-zero on the first timestep
after initialization.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_steady_state_detection/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowSteadyStateDetection

!syntax inputs /Postprocessors/PorousFlowSteadyStateDetection

!syntax children /Postprocessors/PorousFlowSteadyStateDetection
