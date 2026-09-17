# Analytic verification of PorousFlowDoubletSwitch.
#
# PorousFlowDoubletSwitch::execute() latches a charge_time the first time
# abs(targetpostprocessor - temperature_init) >= temperature_tolerance (while charge_time == 0):
#   charge_time = timepostprocessor
# getValue() then returns, once charge_time != 0:
#   temp_time = timepostprocessor - charge_time
#   frac = temp_time/86400 - floor(temp_time/86400)
#   1 if frac < duty_cycle_fraction else 0
# (and 0 unconditionally before charge_time latches).
#
# Drive targetpostprocessor(t) = if(t >= 43200, 10, 0), temperature_init = 0,
# temperature_tolerance = 1, duty_cycle_fraction = 0.5, dt = 21600 s (6 h), num_steps = 8:
#
#   step1 t=21600:  target=0,  diff=0 <1     -> not latched, value=0
#   step2 t=43200:  target=10, diff=10>=1    -> latch charge_time=43200; temp_time=0,      frac=0    <0.5 -> 1
#   step3 t=64800:                              temp_time=21600,  frac=0.25 <0.5 -> 1
#   step4 t=86400:                              temp_time=43200,  frac=0.5  not<0.5 -> 0
#   step5 t=108000:                              temp_time=64800,  frac=0.75 not<0.5 -> 0
#   step6 t=129600:                              temp_time=86400,  frac=0    <0.5 -> 1
#   step7 t=151200:                              temp_time=108000, frac=0.25 <0.5 -> 1
#   step8 t=172800:                              temp_time=129600, frac=0.5  not<0.5 -> 0
#
# so, writing frac(t) = (t-43200)/86400 - floor((t-43200)/86400):
#   exact(t) = if(t < 43200, 0, if(frac(t) < 0.5, 1, 0))

[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions]
  [target_fn]
    type = ParsedFunction
    expression = 'if(t >= 43200, 10, 0)'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 43200, 0, if((t-43200)/86400 - floor((t-43200)/86400) < 0.5, 1, 0))'
  []
[]

[Postprocessors]
  [cur_time]
    type = TimePostprocessor
  []
  [target]
    type = FunctionValuePostprocessor
    function = target_fn
  []
  [computed]
    type = PorousFlowDoubletSwitch
    targetpostprocessor = target
    timepostprocessor = cur_time
    temperature_init = 0
    temperature_tolerance = 1
    duty_cycle_fraction = 0.5
  []
  [exact]
    type = FunctionValuePostprocessor
    function = exact_fn
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'computed exact'
    expression = 'abs(computed - exact)'
  []
[]

[Executioner]
  type = Transient
  dt = 21600
  num_steps = 8
[]

[Outputs]
  csv = true
[]
