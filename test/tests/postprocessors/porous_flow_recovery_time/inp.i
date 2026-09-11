# Analytic verification of PorousFlowRecoveryTime.
#
# PorousFlowRecoveryTime::getValue() returns (timepostprocessor/3600/24 - breakthroughtime),
# i.e. the elapsed time in days since a recorded breakthrough time.
#
# Drive with dt = 86400 s (1 day), num_steps = 5, timepostprocessor = TimePostprocessor
# (reports t in seconds), and breakthroughtime = a constant postprocessor with value 2 (days):
#
#   day 1 (t=86400):  1 - 2 = -1
#   day 2 (t=172800): 2 - 2 =  0
#   day 3 (t=259200): 3 - 2 =  1
#   day 4 (t=345600): 4 - 2 =  2
#   day 5 (t=432000): 5 - 2 =  3
#
# so exact(t) = t/86400 - 2.

[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions]
  [breakthrough_fn]
    type = ParsedFunction
    expression = '2'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 't/86400 - 2'
  []
[]

[Postprocessors]
  [cur_time]
    type = TimePostprocessor
  []
  [breakthrough]
    type = FunctionValuePostprocessor
    function = breakthrough_fn
  []
  [computed]
    type = PorousFlowRecoveryTime
    breakthroughtime = breakthrough
    timepostprocessor = cur_time
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
  dt = 86400
  num_steps = 5
[]

[Outputs]
  csv = true
[]
