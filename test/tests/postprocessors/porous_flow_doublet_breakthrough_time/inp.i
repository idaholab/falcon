# Analytic verification of PorousFlowDoubletBreakthroughTime.
#
# PorousFlowDoubletBreakthroughTime::execute() does:
#   if (keep_constant == 0 && breakthroughterminator >= 0.5)
#     keep_constant = timepostprocessor/3600/24     # latch, in days
# and getValue() returns keep_constant, which stays at whatever it was first latched to.
#
# Drive breakthroughterminator(t) = if(t >= 259200, 1, 0) (a synthetic 0/1 breakthrough flag
# that flips on at t = 3 days = 259200 s), with dt = 86400 s (1 day), num_steps = 5:
#
#   day 1 (t=86400):  flag=0 -> not latched, keep=0
#   day 2 (t=172800): flag=0 -> not latched, keep=0
#   day 3 (t=259200): flag=1 -> first latch: keep = 259200/3600/24 = 3
#   day 4 (t=345600): flag=1, but keep_constant != 0 already -> unchanged, keep=3
#   day 5 (t=432000): flag=1, unchanged, keep=3
#
# so exact(t) = if(t < 259200, 0, 3).

[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions]
  [flag_fn]
    type = ParsedFunction
    expression = 'if(t >= 259200, 1, 0)'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 259200, 0, 3)'
  []
[]

[Postprocessors]
  [cur_time]
    type = TimePostprocessor
  []
  [flag]
    type = FunctionValuePostprocessor
    function = flag_fn
  []
  [computed]
    type = PorousFlowDoubletBreakthroughTime
    breakthroughterminator = flag
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
