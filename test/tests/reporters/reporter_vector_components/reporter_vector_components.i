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
  value = 't * x'
[]

[Postprocessors/pp]
  type = FunctionValuePostprocessor
  function = fun
  point = '1 0 0'
  execute_on = 'initial timestep_end'
[]

[VectorPostprocessors/vpp]
  type = LineFunctionSampler
  functions = fun
  start_point = '0 0 0'
  end_point = '1 0 0'
  num_points = 6
  sort_by = x
  execute_on = 'initial timestep_end'
  outputs = none
[]

[Reporters]
  [rep]
    type = ConstantReporter
    integer_names  = 'int'
    integer_values = '1'
    string_names  = 'str'
    string_values = 'two'
    integer_vector_names  = 'int_vec'
    integer_vector_values = '3 4'
    string_vector_names  = 'str_vec'
    string_vector_values = 'five six seven eight'
    outputs = none
  []
  [components]
    type = ReporterVectorComponents
    reporters = 'vpp/fun rep/int_vec rep/str_vec'
    indices='0 1'
  []
[]

[Executioner]
  type = Transient
  num_steps = 5
[]

[Outputs]
  csv=true
[]
