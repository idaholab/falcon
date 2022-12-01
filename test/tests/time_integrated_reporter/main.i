[Mesh/mesh]
  type = GeneratedMeshGenerator
  dim = 1
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Functions/fun]
  type = ParsedFunction
  value = '1'
[]

[Postprocessors/pp]
  type = FunctionValuePostprocessor
  function = fun
  point = '1 0 0'
  execute_on = 'initial timestep_end'
[]

[Reporters]
  [accumulate]
    type = AccumulateReporter
    reporters = 'pp/value'
  []
[]

[Executioner]
  type = Transient
  num_steps = 5

  fixed_point_max_its = 1
  custom_pp = pp
  direct_pp_value = true
  disable_fixed_point_residual_norm_check = true
  accept_on_max_fixed_point_iteration = true
[]

[Postprocessors]
  [int_value]
    type = TimeIntegratedReporterValue
    reporter = 'accumulate/pp:value'
    use_negative_value = true
  []
[]

[Outputs]
  [out]
    type = CSV
  []
[]
