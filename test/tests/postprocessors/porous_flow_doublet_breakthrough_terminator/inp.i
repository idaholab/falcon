# Analytic verification of PorousFlowDoubletBreakthroughTerminator, paired with a
# [UserObjects] Terminator to demonstrate that it can actually stop a run.
#
# PorousFlowDoubletBreakthroughTerminator::execute() does:
#   keep_constant = 1 if abs(targetpostprocessor - temperature_init) >= temperature_tolerance
#                   else 0
# and getValue() returns keep_constant (a 0/1 breakthrough indicator).
#
# Drive targetpostprocessor(t) = temperature_init + 0.25*t = 273.15 + 0.25*t, with
# temperature_tolerance = 0.5 (dt = 1, integer times t = 1..8):
#
#   diff(t) = abs(0.25*t) = 0.25*t
#   t=1: diff=0.25 <0.5 -> 0
#   t=2: diff=0.50 >=0.5 -> 1   (breakthrough first detected)
#   t=3..8: diff>0.5 -> 1
#
# so exact(t) = if(t < 2, 0, 1).
#
# A [UserObjects] Terminator is paired on 'breakthrough >= 1' with num_steps = 8, well past the
# t=2 flip, so the run hard-stops right after the flip and the gold CSV has only 2 data rows
# (plus the initial row) instead of 8.

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
    expression = '273.15 + 0.25*t'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 2, 0, 1)'
  []
[]

[Postprocessors]
  [target]
    type = FunctionValuePostprocessor
    function = target_fn
  []
  [breakthrough]
    type = PorousFlowDoubletBreakthroughTerminator
    targetpostprocessor = target
    temperature_init = 273.15
    temperature_tolerance = 0.5
  []
  [exact]
    type = FunctionValuePostprocessor
    function = exact_fn
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'breakthrough exact'
    expression = 'abs(breakthrough - exact)'
  []
[]

[UserObjects]
  [terminate]
    type = Terminator
    expression = 'breakthrough >= 1'
  []
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 8
[]

[Outputs]
  csv = true
[]
