[Mesh]
  type = FileMesh
  dim = 2
  file = steady_out.e
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = u
    initial_from_file_timestep = 2
  [../]
[]

[Kernels]
  [./bodyforce]
    type = BodyForce
    variable = u
    value = 10.0
  [../]
  [./ie]
    type = TimeDerivative
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Materials]
  [./constant]
    type = Constant
    block = 0
  [../]
[]

[Executioner]
  type = Transient

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  start_time = 0.0
  num_steps = 10
  dt = .1
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
