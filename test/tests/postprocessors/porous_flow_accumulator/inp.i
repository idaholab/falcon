# Analytic verification of PorousFlowAccumulator.
#
# PorousFlowAccumulator::execute() does:
#   if (accumulator_start_time < time < accumulator_end_time)
#     if (targetpostprocessor < 0)
#       accumulator += targetpostprocessor
# and getValue() returns abs(accumulator).
#
# Drive the target postprocessor with target(t) = 3 - t (dt = 1, integer times
# t = 1..6) and restrict accumulation to the open window (2, 5):
#
#   t=1: value= 2  (positive -> skipped)
#   t=2: value= 1  (positive -> skipped)
#   t=3: value= 0  (not < 0  -> skipped, also window edge t=2 is excluded)
#   t=4: value=-1  (window 2<4<5 true, negative -> accumulate: acc=-1)
#   t=5: value=-2  (window 2<5<5 FALSE (upper bound excluded) -> skipped)
#   t=6: value=-3  (window 2<6<5 FALSE -> skipped)
#
# so acc stays at -1 for t>=4 and the postprocessor value (=abs(acc)) is
# exactly 0 for t=1,2,3 and 1 for t=4,5,6, i.e. exact(t) = if(t<4, 0, 1).

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
    expression = '3 - t'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 4, 0, 1)'
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
    type = PorousFlowAccumulator
    targetpostprocessor = target
    timepostprocessor = cur_time
    accumulator_start_time = 2
    accumulator_end_time = 5
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
