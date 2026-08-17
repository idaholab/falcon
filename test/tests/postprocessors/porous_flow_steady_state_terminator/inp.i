# Analytic verification of PorousFlowSteadyStateTerminator, paired with a [UserObjects]
# Terminator to demonstrate that it can actually stop a run.
#
# PorousFlowSteadyStateTerminator::getValue() returns:
#   1 if (targetpostprocessor <= ss_relative_error AND timepostprocessor > ss_detection_start_time)
#        OR timepostprocessor > ss_detection_end_time
#   0 otherwise
#
# Drive targetpostprocessor(t) = 1/t (a synthetic, monotonically decreasing steady-state
# detection metric), ss_relative_error = 0.2, ss_detection_start_time = 0,
# ss_detection_end_time = 1000 (so large it is never reached in this test, isolating the first
# OR branch), dt = 1, num_steps = 8:
#
#   t=1: 1/1=1     >0.2 -> 0
#   t=2: 1/2=0.5   >0.2 -> 0
#   t=3: 1/3=0.333 >0.2 -> 0
#   t=4: 1/4=0.25  >0.2 -> 0
#   t=5: 1/5=0.2  <=0.2 -> 1   (first time the detection metric reaches the threshold)
#   t=6,7,8: 1/t  <=0.2 -> 1
#
# so exact(t) = if(t < 5, 0, 1).
#
# A [UserObjects] Terminator is paired on 'computed >= 1' with num_steps = 8, well past the t=5
# flip, so the run hard-stops right after the flip and the gold CSV has only 5 data rows (plus
# the initial row) instead of 8.

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
    expression = '1/t'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 5, 0, 1)'
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
    type = PorousFlowSteadyStateTerminator
    targetpostprocessor = target
    timepostprocessor = cur_time
    ss_detection_start_time = 0
    ss_detection_end_time = 1000
    ss_relative_error = 0.2
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

[UserObjects]
  [terminate]
    type = Terminator
    expression = 'computed >= 1'
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
