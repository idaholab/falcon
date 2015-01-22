# Solves a pair of coupled diffusion equations where u=v on the boundary

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
    initial_condition = 3
  [../]
  [./v]
    order = FIRST
    family = LAGRANGE
    initial_condition = 2
  [../]
[]

[Kernels]
  [./diff_u]
    type = Diffusion
    variable = u
  [../]
  [./diff_v]
    type = Diffusion
    variable = v
  [../]
[]

[BCs]
  [./right_v]
    type = DirichletBC
    variable = v
    boundary = right
    value = 3
  [../]
  [./left_u]
    type = MatchedValueBC
    variable = u
    boundary = left
    v = v
    value = 3
  [../]
[]

[Materials]
  [./constant]
    type = Constant
    block = 0
  [../]
[]

[Executioner]
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-15
  l_tol = 1e-12
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  print_perf_log = true
[]
