# PorousFlowAccumulator

!syntax description /Postprocessors/PorousFlowAccumulator

## Description

`PorousFlowAccumulator` accumulates the value of a target postprocessor over the course of a
transient simulation, but only counts a contribution on a timestep if:

- the target postprocessor's current value is negative, and
- the current simulation time (as reported by a supplied time postprocessor) lies strictly
  between `accumulator_start_time` and `accumulator_end_time`.

The running sum is kept internally, and the postprocessor value reported is the absolute value
of that sum. This is typically used to track a cumulative quantity
(e.g. injected fluid mass or energy, which is reported as negative by convention) over a
specific portion of a simulation, ignoring both the sign convention and any activity outside the
time window of interest.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_accumulator/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowAccumulator

!syntax inputs /Postprocessors/PorousFlowAccumulator

!syntax children /Postprocessors/PorousFlowAccumulator
