# Solves a pair of coupled diffusion equations where u=v on the boundary

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
[]

[Variables]
  [./u]
    initial_condition = 1
    block = 0
  [../]
[]

[Functions]
  [./step_func_test]
    type = StepPressureBCFunc
    p2 = 2
    p3 = 3
    p1 = 1
    p4 = 4
    t2 = 3
    t3 = 4
    t1 = 2
  [../]
  [./sin_func]
    type = ParsedFunction
    value = sin(y)
  [../]
[]

[Kernels]
  [./diff_u]
    type = Diffusion
    variable = u
    block = 0
  [../]
[]

[BCs]
  [./left_u]
    type = FunctionDirichletBC
    variable = u
    boundary = left
    function = sin_func
  [../]
  [./top_u]
    type = FunctionDirichletBC
    variable = u
    boundary = top
    function = step_func_test
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
  num_steps = 4
  dt = 1
  petsc_options = -snes_mf_operator
  nl_rel_tol = 1e-15
  l_tol = 1e-12
[]

[Output]
  file_base = out
  output_initial = true
  exodus = true
  perf_log = true
[]

