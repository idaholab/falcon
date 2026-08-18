# Analytic verification of PorousFlowRecoveryRateSeason.
#
# PorousFlowRecoveryRateSeason::execute() does:
#   if (InjectionIndicator == 1)
#     accumulator_inj += (hotwellenergy + coldwellenergy)
#   else if (ProductionIndicator == 1)
#     accumulator_ext += (hotwellenergy + coldwellenergy)
# and getValue() returns 0 if accumulator_ext == 0, else abs(ext)/abs(inj)*100.
#
# Drive hotwellenergy = 2, coldwellenergy = 1 (sum = 3, constant), dt = 1, num_steps = 6,
# InjectionIndicator(t) = 1 for t<=3 (else 0), ProductionIndicator(t) = 1 for t>3 (else 0):
#
#   t=1,2,3: injection season -> inj += 3 each step -> inj = 3,6,9 ; ext stays 0 -> rate=0
#   t=4:     production season -> ext += 3          -> ext = 3  ; inj = 9 (fixed) -> rate = 3/9*100
#   t=5:     production season -> ext += 3          -> ext = 6  ; rate = 6/9*100
#   t=6:     production season -> ext += 3          -> ext = 9  ; rate = 9/9*100
#
# so exact(t) = if(t<=3, 0, (100/3)*(t-3)).

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
    expression = '2'
  []
  [cold_fn]
    type = ParsedFunction
    expression = '1'
  []
  [inj_fn]
    type = ParsedFunction
    expression = 'if(t <= 3, 1, 0)'
  []
  [pro_fn]
    type = ParsedFunction
    expression = 'if(t <= 3, 0, 1)'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t <= 3, 0, (100/3)*(t - 3))'
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
  [inj_indicator]
    type = FunctionValuePostprocessor
    function = inj_fn
  []
  [pro_indicator]
    type = FunctionValuePostprocessor
    function = pro_fn
  []
  [computed]
    type = PorousFlowRecoveryRateSeason
    hotwellenergy = hot
    coldwellenergy = cold
    InjectionIndicator = inj_indicator
    ProductionIndicator = pro_indicator
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
