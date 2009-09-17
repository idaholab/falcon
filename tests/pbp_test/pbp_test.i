[Mesh]
  dim = 2
  file = square.e
#  init_unif_refine = 6
[]

[Variables]
  names = 'u v'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]

  [./v]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  names = 'diff_u conv_v diff_v'

  [./diff_u]
    type = Diffusion
    variable = u
  [../]

  [./conv_v]
    type = CoupledForce
    variable = v
    coupled_to = u
    coupled_as = v
  [../]

  [./diff_v]
    type = Diffusion
    variable = v
  [../]
[]

[BCs]
  names = 'left_u right_u left_v'

  [./left_u]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0
  [../]

  [./right_u]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 100
  [../]

  [./left_v]
    type = DirichletBC
    variable = v
    boundary = 1
    value = 0
  [../]

  [./right_v]
    type = DirichletBC
    variable = v
    boundary = 2
    value = 0
  [../]
[]

[Materials]
  names = constant

  [./constant]
    type = Constant
    block = 1
  [../]
[]

[Execution]
  type = Steady
  perf_log = true

  l_max_its = 1
  nl_max_its = 1

  petsc_options = '-snes_mf'
  pbp = true
  [./PBP]
    solve_order = 'u v'
    preconditioner  = 'LU LU'
    off_diag_row    = 'v'
    off_diag_column = 'u'
  [../]
[]

[Output]
  file_base = out
  interval = 1
  exodus = true
[]
   
    
