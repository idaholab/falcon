# TimestepPostprocessor

!syntax description /Postprocessors/TimestepPostprocessor

## Description

`TimestepPostprocessor` selects between two timestep sizes based on whether a target
postprocessor has crossed a tolerance, typically used to shrink the timestep once a significant
event (e.g. thermal breakthrough) has been detected. On the first timestep where
`targetpostprocessor` reaches `enthalpy_relative_tolerance`, an internal trigger time is latched
to the current simulation time (`timepostprocessor`); this latch is permanent for the rest of
the simulation. Before the trigger latches, the reported value is `dt_before_trigger` (default
2592000 s, i.e. one month); from the triggering timestep onward (inclusive), it is
`dt_after_trigger` (default 10800 s, i.e. six hours).

Because this postprocessor merely *reports* a value, it must be supplied as the `postprocessor`
parameter of a `PostprocessorDT` (or similar) `[TimeSteppers]`/`[Executioner]` timestep sizing
mechanism to actually control the simulation's timestep.

## Example Input Syntax

!listing test/tests/postprocessors/timestep_postprocessor/inp.i block=Postprocessors

!syntax parameters /Postprocessors/TimestepPostprocessor

!syntax inputs /Postprocessors/TimestepPostprocessor

!syntax children /Postprocessors/TimestepPostprocessor
