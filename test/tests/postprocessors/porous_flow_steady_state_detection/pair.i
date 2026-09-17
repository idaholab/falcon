# Regression test for PorousFlowSteadyStateDetection wired to PorousFlowSteadyStateTerminator --
# the pairing both doc pages recommend, and which no single input exercised before.
#
# PorousFlowSteadyStateDetection cannot compute a relative rate of change on the first timestep
# (TimestepSize's "old" value is 0 there), nor whenever the target's old value is 0 (the normal
# state of a cumulative or integral quantity at t0). For those cases it reports a large sentinel,
# 1e30.
#
# It must NOT report 0. The terminator below treats a SMALL value as converged, so with its default
# ss_relative_error = 1e-2 and ss_detection_start_time = 0, a 0 would satisfy
#   value <= ss_relative_error && t > ss_detection_start_time
# on the very first step, and the Terminator UserObject would hard-stop the run one step in --
# reporting an instantly-converged steady state. Both parameters are deliberately left at their
# defaults here, because it is the defaults that make that the observable failure.
#
# The target is deliberately far from steady: target(t) = 5 + t gives a relative rate of change of
# 1/(4 + t), which is still 0.1 at the last step -- an order of magnitude above ss_relative_error.
# The terminator must therefore never fire and the run must reach all 6 steps.
#
# If the sentinel ever regresses to 0, the Terminator fires at t = 1, the CSV is truncated, and
# CSVDiff's row-count check turns that into a failure.

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
[]

[Postprocessors]
  [dt_pp]
    type = TimestepSize
    execute_on = 'initial timestep_end'
  []
  [cur_time]
    type = TimePostprocessor
  []
  [target]
    type = FunctionValuePostprocessor
    function = target_fn
    execute_on = 'initial timestep_end'
  []
  [ss_detect]
    type = PorousFlowSteadyStateDetection
    targetpostprocessor = target
    dt_postprocessor = dt_pp
  []
  [ss_term]
    type = PorousFlowSteadyStateTerminator
    targetpostprocessor = ss_detect
    timepostprocessor = cur_time
    # ss_relative_error, ss_detection_start_time and ss_detection_end_time all left at defaults.
  []
[]

[UserObjects]
  [terminate_on_steady_state]
    type = Terminator
    expression = 'ss_term > 0.5'
    message = 'PorousFlowSteadyStateTerminator reported steady state'
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
