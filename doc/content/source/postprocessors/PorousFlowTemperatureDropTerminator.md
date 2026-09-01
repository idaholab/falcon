# PorousFlowTemperatureDropTerminator

!syntax description /Postprocessors/PorousFlowTemperatureDropTerminator

## Description

`PorousFlowTemperatureDropTerminator` reports a `0`/`1` indicator of whether a produced fluid's
temperature has dropped by a set percentage of the way from the reservoir's initial temperature
towards the injection temperature. The produced temperature is reconstructed from a specific
enthalpy postprocessor (`enthalpypostprocessor`, in J) and a mass postprocessor
(`masspostprocessor`, in kg) as `T_prod = enthalpypostprocessor/masspostprocessor/4186 + 273.15`
(using 4186 J/(kg K) as an approximate specific heat of water to convert enthalpy to a
temperature rise, and 273.15 to convert to Kelvin). The reported value is `1` once

```
(T_prod - T_init)/(T_inj - T_init) < P_drop/100
```

and also `1`, unconditionally, once the current simulation time (`timepostprocessor`) reaches
`max_time`, regardless of the temperature drop; this second condition guarantees the terminator
eventually fires even if the reservoir is far from equilibrating with the injection temperature.
Otherwise the reported value is `0`.

This postprocessor only *reports* the indicator; it does not stop the simulation by itself. To
actually terminate the run once the temperature drop criterion is met, pair it with a
[Terminator](Terminator.md) `[UserObjects]` block whose `expression` tests this postprocessor's
value, as shown in the example below.

## Example Input Syntax

!listing test/tests/postprocessors/porous_flow_temperature_drop_terminator/inp.i block=Postprocessors

!listing test/tests/postprocessors/porous_flow_temperature_drop_terminator/inp.i block=UserObjects

!syntax parameters /Postprocessors/PorousFlowTemperatureDropTerminator

!syntax inputs /Postprocessors/PorousFlowTemperatureDropTerminator

!syntax children /Postprocessors/PorousFlowTemperatureDropTerminator
