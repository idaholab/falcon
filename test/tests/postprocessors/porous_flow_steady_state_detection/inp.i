# Analytic verification of PorousFlowSteadyStateDetection.
#
# PorousFlowSteadyStateDetection::getValue() returns the sentinel 1e30 at t_step == 0, or whenever
# dt, dt_old, or value_old is 0 (division guard), and otherwise
#   abs( (value/dt - value_old/dt_old) / (value_old/dt_old) )
# where value/value_old and dt/dt_old are the current/old values of the target and dt
# postprocessors.
#
# Drive with dt_postprocessor = TimestepSize and targetpostprocessor(t) = 5 + t, so that
# value_old is always 1 less than value. TimestepSize's value at the initial time (before the
# Transient executioner has set a timestep size) is 0, so the "old" dt recorded for the first
# timestep (t_step=1) is 0 -- this exercises the division guard directly, rather than stepping
# around it with a constant dt postprocessor.
#
#   t_step=0 (t=0): the guard applies (not evaluated into the CSV, since this postprocessor's own
#                   execute_on is the default, timestep_end only)
#   t_step=1 (t=1): dt_old=0, so the division guard reports the 1e30 sentinel
#   t_step=k (t=k), k=2..6: dt=dt_old=1 (cancels), value=5+k, value_old=5+(k-1)=4+k
#                   change = (value-value_old)/value_old = 1/(4+k)
#
# so exact(t) = 1e30 for t=1, and 1/(4+t) for t = 2..6 (recalling dt=1 so t and t_step coincide
# numerically). The sentinel must be large, not 0: see pair.i, which pins down why.

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
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t <= 1, 1e30, 1/(4 + t))'
  []
[]

[Postprocessors]
  [dt_pp]
    type = TimestepSize
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
    dt_postprocessor = dt_pp
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
