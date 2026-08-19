# Analytic verification of PorousFlowRecoveryRate.
#
# PorousFlowRecoveryRate::execute() does:
#   if (hotwellenergy <= 0)
#     accumulator_inj += (hotwellenergy + coldwellenergy)   # injection leg
#   else
#     accumulator_ext += (hotwellenergy + coldwellenergy)   # extraction leg
# and getValue() returns 0 if accumulator_ext == 0, else abs(ext)/abs(inj)*100.
#
# Drive coldwellenergy = 0 and hotwellenergy(t) = -2 for t<=3 (injection), +3 for t>3
# (extraction), dt = 1, num_steps = 6:
#
#   t=1,2,3: hot=-2<=0 -> inj += -2 each step -> inj = -2,-4,-6 ; ext stays 0 -> rate=0
#   t=4:     hot=3>0   -> ext += 3            -> ext = 3  ; inj = -6 -> rate = 3/6*100  = 50
#   t=5:     hot=3>0   -> ext += 3            -> ext = 6  ; inj = -6 -> rate = 6/6*100  = 100
#   t=6:     hot=3>0   -> ext += 3            -> ext = 9  ; inj = -6 -> rate = 9/6*100  = 150
#
# so exact(t) = if(t<=3, 0, 50*(t-3)).

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
    expression = 'if(t <= 3, -2, 3)'
  []
  [cold_fn]
    type = ParsedFunction
    expression = '0'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t <= 3, 0, 50*(t - 3))'
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
  [computed]
    type = PorousFlowRecoveryRate
    hotwellenergy = hot
    coldwellenergy = cold
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
