# Analytic verification of PorousFlowEnergyAccumulator.
#
# PorousFlowEnergyAccumulator::execute() does:
#   if (ProductionIndicator == 1)
#     accumulator += (hotwellenergy + coldwellenergy)
# and getValue() returns abs(accumulator).
#
# Drive hotwellenergy(t) = t, coldwellenergy(t) = 1 (dt = 1, integer times
# t = 1..6), and ProductionIndicator(t) = 1 for t<=3, 0 for t>3:
#
#   t=1: indicator=1 -> acc += (1+1) = 2   -> acc=2
#   t=2: indicator=1 -> acc += (2+1) = 3   -> acc=5
#   t=3: indicator=1 -> acc += (3+1) = 4   -> acc=9
#   t=4: indicator=0 -> skipped            -> acc=9
#   t=5: indicator=0 -> skipped            -> acc=9
#   t=6: indicator=0 -> skipped            -> acc=9
#
# For t<=3, acc(t) = sum_{i=1}^{t} (i+1) = (t^2 + 3t)/2, and acc is constant
# at 9 for t>3, i.e. exact(t) = if(t<=3, (t*t+3*t)/2, 9).

[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions]
  [hot_fn]
    type = ParsedFunction
    expression = 't'
  []
  [cold_fn]
    type = ParsedFunction
    expression = '1'
  []
  [indicator_fn]
    type = ParsedFunction
    expression = 'if(t <= 3, 1, 0)'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t <= 3, (t*t + 3*t)/2, 9)'
  []
[]

[Postprocessors]
  [hot]
    type = FunctionValuePostprocessor
    function = hot_fn
  []
  [cold]
    type = FunctionValuePostprocessor
    function = cold_fn
  []
  [indicator]
    type = FunctionValuePostprocessor
    function = indicator_fn
  []
  [computed]
    type = PorousFlowEnergyAccumulator
    hotwellenergy = hot
    coldwellenergy = cold
    ProductionIndicator = indicator
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
