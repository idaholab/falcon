# FalconPostprocessorFunction

!syntax description /Functions/FalconPostprocessorFunction

## Description

`FalconPostprocessorFunction` returns the current value of the postprocessor named by `pp`,
unconditionally, ignoring its own `t`/`x` arguments. It exists to feed a computed scalar into
any parameter typed `FunctionName` - for example [PorousFlowRateControlledBoreholePressure.md]'s
corrected bottomhole pressure into `PorousFlowPeacemanBorehole`'s `bottom_p_or_t`, which accepts
only a `Function`, not a `Postprocessor`, directly.

This is a straightforward port of MOOSE's own `PostprocessorFunction`
(`moose/test/include/functions/PostprocessorFunction.h`), which is registered only under
`MooseTestApp` and so is not available to a FALCON input file.

## Example Input Syntax

!listing examples/geothermal_wellbore/wells_injection.i block=Functions/bottomhole_pressure_control

!syntax parameters /Functions/FalconPostprocessorFunction

!syntax inputs /Functions/FalconPostprocessorFunction

!syntax children /Functions/FalconPostprocessorFunction
