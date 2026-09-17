# Analytic verification of TimestepPostprocessor.
#
# TimestepPostprocessor::execute() latches a boolean trigger the first time
# targetpostprocessor >= enthalpy_relative_tolerance:
#   triggered = true
# getValue() then returns dt_before_trigger while !triggered, or dt_after_trigger once the
# trigger has latched (note triggered is set in execute() before getValue() is called on the
# same timestep, so the trigger step itself already reports dt_after_trigger).
#
# Drive targetpostprocessor(t) = t, enthalpy_relative_tolerance = 3.5,
# dt_before_trigger = 100, dt_after_trigger = 10, dt = 1, num_steps = 6:
#
#   t=1,2,3: value=1,2,3 < 3.5 -> not latched      -> getValue = 100
#   t=4:     value=4 >= 3.5 -> latch triggered     -> getValue = 10  (trigger step)
#   t=5,6:   already latched                        -> getValue = 10
#
# so exact(t) = if(t < 4, 100, 10).

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
    expression = 't'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 4, 100, 10)'
  []
[]

[Postprocessors]
  [target]
    type = FunctionValuePostprocessor
    function = target_fn
  []
  [computed]
    type = TimestepPostprocessor
    targetpostprocessor = target
    enthalpy_relative_tolerance = 3.5
    dt_before_trigger = 100
    dt_after_trigger = 10
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
  dt = 1
  num_steps = 6
[]

[Outputs]
  csv = true
[]
