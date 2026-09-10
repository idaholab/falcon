# PorousFlowSteadyStateDetection

!syntax description /Postprocessors/PorousFlowSteadyStateDetection

## Description

`PorousFlowSteadyStateDetection` computes the relative rate of change, between the current and
previous timestep, of a target postprocessor's value normalized by the timestep size. Denoting
the current and old values of `targetpostprocessor` as `value`/`value_old` and of
`dt_postprocessor` (the timestep-size postprocessor) as `dt`/`dt_old`, the reported value is

```
abs( (value/dt - value_old/dt_old) / (value_old/dt_old) )
```

except on the very first (`t_step == 0`) evaluation, or whenever `dt`, `dt_old`, or `value_old`
is `0` (a division guard), where it instead reports the large sentinel `1e30`. This quantity
trends toward zero as a simulation approaches a steady state, and is typically fed into
[PorousFlowSteadyStateTerminator.md] to stop a transient once the system has settled.

The sentinel matters for that pairing: `PorousFlowSteadyStateTerminator` treats a *small* value as
"converged", so a "cannot compute yet" result has to be large rather than `0`. Reporting `0` here
would make the terminator fire on the very first timestep and hard-stop the run, looking like an
instantly-converged steady state. `1e30` reads unambiguously as *not* steady, and matches the
sentinel used for `ss_detection_end_time`.

Because `value_old` and `dt_old` are only meaningful once a postprocessor has been through at
least one real timestep transition, feeding this postprocessor a `dt_postprocessor` whose value
is not yet defined at the simulation's initial time (e.g. `TimestepSize`, whose value before the
executioner has set a timestep is `0`) reports the sentinel on the first timestep after
initialization rather than dividing by zero. The same applies to a cumulative or integral
`targetpostprocessor`, whose `value_old` is `0` at `t0`.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_steady_state_detection/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowSteadyStateDetection

!syntax inputs /Postprocessors/PorousFlowSteadyStateDetection

!syntax children /Postprocessors/PorousFlowSteadyStateDetection
