# PorousFlowDoubletSwitch

!syntax description /Postprocessors/PorousFlowDoubletSwitch

## Description

`PorousFlowDoubletSwitch` reports a `0`/`1` daily duty-cycle indicator for operating a doublet
system, intended to be used once thermal breakthrough has occurred. Before breakthrough is
detected the reported value is always `0`. Breakthrough is detected (and latched) the first time
the absolute difference between `targetpostprocessor` and `temperature_init` reaches
`temperature_tolerance`; from that point on, each day is split into an "on" fraction (the first
`duty_cycle_fraction` of the day since breakthrough, default `0.6666667`, i.e. two thirds of the
day) during which `1` is reported, and an "off" fraction during which `0` is reported.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_doublet_switch/inp.i block=Postprocessors

!syntax parameters /Postprocessors/PorousFlowDoubletSwitch

!syntax inputs /Postprocessors/PorousFlowDoubletSwitch

!syntax children /Postprocessors/PorousFlowDoubletSwitch
