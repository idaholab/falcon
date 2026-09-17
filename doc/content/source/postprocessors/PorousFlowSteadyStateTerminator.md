# PorousFlowSteadyStateTerminator

!syntax description /Postprocessors/PorousFlowSteadyStateTerminator

## Description

`PorousFlowSteadyStateTerminator` reports a `0`/`1` steady-state indicator, typically fed a
detection metric produced by [PorousFlowSteadyStateDetection.md]. The reported value is `1` if
either:

- the current simulation time (`timepostprocessor`) is past `ss_detection_start_time` and
  `targetpostprocessor` is at or below `ss_relative_error`, or
- the current simulation time is past `ss_detection_end_time` (regardless of
  `targetpostprocessor`, acting as a time-based fallback in case steady state is never detected);

and `0` otherwise.

This postprocessor only *reports* the indicator; it does not stop the simulation by itself. To
actually terminate the run once steady state is detected, pair it with a
[Terminator](Terminator.md) `[UserObjects]` block whose `expression` tests this postprocessor's
value, as shown in the example below.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_steady_state_terminator/inp.i block=Postprocessors

!listing test/tests/postprocessors/porous_flow_steady_state_terminator/inp.i block=UserObjects

!syntax parameters /Postprocessors/PorousFlowSteadyStateTerminator

!syntax inputs /Postprocessors/PorousFlowSteadyStateTerminator

!syntax children /Postprocessors/PorousFlowSteadyStateTerminator
