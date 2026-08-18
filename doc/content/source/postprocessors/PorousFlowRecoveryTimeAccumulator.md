# PorousFlowRecoveryTimeAccumulator

!syntax description /Postprocessors/PorousFlowRecoveryTimeAccumulator

## Description

`PorousFlowRecoveryTimeAccumulator` accumulates elapsed operating time, in days, over the
portion of a transient simulation during which an indicator postprocessor (`targetpostprocessor`,
e.g. a winter/summer season flag) is truthy (non-zero). On every timestep where the indicator is
non-zero, the current timestep size (`dtpostprocessor`, in seconds) is added to a running total;
the postprocessor value reported is the absolute value of that running total divided by 86400,
i.e. the accumulated time expressed in days.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_recovery_time_accumulator/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowRecoveryTimeAccumulator

!syntax inputs /Postprocessors/PorousFlowRecoveryTimeAccumulator

!syntax children /Postprocessors/PorousFlowRecoveryTimeAccumulator
