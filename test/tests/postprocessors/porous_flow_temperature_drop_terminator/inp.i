# Analytic verification of PorousFlowTemperatureDropTerminator, paired with a [UserObjects]
# Terminator to demonstrate that it can actually stop a run.
#
# PorousFlowTemperatureDropTerminator::getValue() returns:
#   1 if timepostprocessor >= max_time
#   0 if masspostprocessor == 0 or T_inj == T_init (division guard)
#   1 if ((enthalpypostprocessor/masspostprocessor/fluid_specific_heat + T_enthalpy_ref - T_init)
#            / (T_inj - T_init)) < P_drop/100
#   0 otherwise
#
# i.e. writing T_prod = enthalpypostprocessor/masspostprocessor/fluid_specific_heat +
# T_enthalpy_ref (a produced-fluid temperature reconstructed from specific enthalpy) and
# ratio = (T_prod-T_init)/(T_inj-T_init), the postprocessor fires once ratio drops below
# P_drop/100. fluid_specific_heat is deliberately set to 4000 here (not the class default of
# 4186) so the test cannot pass merely because the analytic verification and the implementation
# happen to share the same hard-coded constant.
#
# Drive masspostprocessor = 1 (constant, except at t=0 -- see below), T_init = 0, T_inj = 100,
# P_drop = 50 (so the threshold is 0.5), max_time huge (so the max_time branch never fires here,
# isolating the ratio-threshold branch), and choose the enthalpy function so that
#   T_prod(t) = 100 - 10*t   =>   ratio(t) = T_prod(t)/100 = 1 - 0.1*t
# i.e. enthalpypostprocessor(t) = masspostprocessor * 4000 * (T_prod(t) - 273.15)
#                                = 4000*(100 - 10*t - 273.15), with dt = 1, num_steps = 10:
#
#   t=0:    masspostprocessor's own execute_on default excludes 'initial', so both the mass and
#           enthalpy postprocessors report their zero-initialized value here -- this exercises
#           the masspostprocessor == 0 division guard directly (division by the true, unguarded
#           0/0 would be NaN, and NaN compares false against P_drop/100, which would have
#           masked this guard by accident) -> 0
#   t=1..5: ratio = 1-0.1t >= 0.5   -> 0
#   t=5:    ratio = 1-0.5 = 0.5, NOT < 0.5 (strict inequality)                -> 0
#   t=6:    ratio = 1-0.6 = 0.4 < 0.5                                        -> 1  (first fire)
#   t=7..10: ratio keeps decreasing, still < 0.5                            -> 1
#
# so exact(t) = if(t < 6, 0, 1).
#
# A [UserObjects] Terminator is paired on 'computed >= 1' with num_steps = 10, well past the
# t=6 flip, so the run hard-stops right after the flip and the gold CSV has only 6 data rows
# (plus the initial row) instead of 10.

[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions]
  [enthalpy_fn]
    type = ParsedFunction
    expression = '4000*(100 - 10*t - 273.15)'
  []
  [mass_fn]
    type = ParsedFunction
    expression = '1'
  []
  [exact_fn]
    type = ParsedFunction
    expression = 'if(t < 6, 0, 1)'
  []
[]

[Postprocessors]
  [cur_time]
    type = TimePostprocessor
  []
  [enthalpy]
    type = FunctionValuePostprocessor
    function = enthalpy_fn
  []
  [mass]
    type = FunctionValuePostprocessor
    function = mass_fn
  []
  [computed]
    type = PorousFlowTemperatureDropTerminator
    enthalpypostprocessor = enthalpy
    masspostprocessor = mass
    timepostprocessor = cur_time
    T_inj = 100
    T_init = 0
    P_drop = 50
    max_time = 1e6
    fluid_specific_heat = 4000
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
  num_steps = 10
[]

[Outputs]
  csv = true
[]
