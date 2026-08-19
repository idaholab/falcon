# Analytic verification of PorousFlowSteadyStateDetection.
#
# PorousFlowSteadyStateDetection::getValue() returns 0 at t_step == 0, and otherwise
#   abs( (value/dt - value_old/dt_old) / (value_old/dt_old) )
# where value/value_old and dt/dt_old are the current/old values of the target and dt
# postprocessors.
#
# Drive with a constant dt postprocessor (value 1, so dt == dt_old always, cancelling out of
# the expression) and targetpostprocessor(t) = 5 + t, so that value_old is always 1 less than
# value. Both the target and dt postprocessors are executed on 'initial timestep_end' so that a
# valid "old" value (recorded at t=0) is available for the first timestep (t_step=1). Note the dt
# postprocessor must be a constant rather than e.g. TimestepSize: TimestepSize's value at the
# initial time (before the Transient executioner has set a timestep size) is 0, which would make
# the "old" dt recorded for the first timestep 0, causing a genuine division by zero the first
# time this class's formula is evaluated.
#
#   t_step=0 (t=0): forced to 0 by the class (not written to the CSV since this postprocessor's
#                   own execute_on is the default, timestep_end only)
#   t_step=k (t=k), k=1..6: value=5+k, value_old=5+(k-1)=4+k
#                   change = (value-value_old)/value_old = 1/(4+k)
#
# so exact(t) = 1/(4+t) for t = 1..6 (recalling dt=1 so t and t_step coincide numerically).

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
    expression = '5 + t'
  []
  [dt_fn]
    type = ParsedFunction
    expression = '1'
  []
  [exact_fn]
    type = ParsedFunction
    expression = '1/(4 + t)'
  []
[]

[Postprocessors]
  [dt_pp]
    type = FunctionValuePostprocessor
    function = dt_fn
    execute_on = 'initial timestep_end'
  []
  [target]
    type = FunctionValuePostprocessor
    function = target_fn
    execute_on = 'initial timestep_end'
  []
  [computed]
    type = PorousFlowSteadyStateDetection
    targetpostprocessor = target
    timepostprocessor = dt_pp
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
