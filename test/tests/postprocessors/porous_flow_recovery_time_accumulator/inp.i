# Analytic verification of PorousFlowRecoveryTimeAccumulator.
#
# PorousFlowRecoveryTimeAccumulator::execute() does:
#   if (targetpostprocessor)          # truthy, i.e. non-zero
#     accumulator += dtpostprocessor
# and getValue() returns abs(accumulator/3600/24), i.e. the accumulated time in days.
#
# Drive with a fixed dt = 86400 s (1 day), num_steps = 6, dtpostprocessor = TimestepSize
# (always 86400), and targetpostprocessor(t) = 1 for t <= 4 days, 0 afterwards
# (t is in seconds, so the cutoff is at t = 4*86400 = 345600 s):
#
#   day 1 (t=86400):  indicator=1 -> acc += 86400   -> acc=86400   (1 day)
#   day 2 (t=172800): indicator=1 -> acc += 86400   -> acc=172800  (2 days)
#   day 3 (t=259200): indicator=1 -> acc += 86400   -> acc=259200  (3 days)
#   day 4 (t=345600): indicator=1 -> acc += 86400   -> acc=345600  (4 days)
#   day 5 (t=432000): indicator=0 -> skipped        -> acc=345600  (4 days)
#   day 6 (t=518400): indicator=0 -> skipped        -> acc=345600  (4 days)
#
# so exact(t) [days] = if(t <= 345600, t/86400, 4).

[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions]
  [indicator_fn]
    type = ParsedFunction
    expression = 'if(t <= 345600, 1, 0)'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t <= 345600, t/86400, 4)'
  []
[]

[Postprocessors]
  [dt_pp]
    type = TimestepSize
  []
  [indicator]
    type = FunctionValuePostprocessor
    function = indicator_fn
  []
  [computed]
    type = PorousFlowRecoveryTimeAccumulator
    targetpostprocessor = indicator
    dtpostprocessor = dt_pp
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
  num_steps = 6
[]

[Outputs]
  csv = true
[]
