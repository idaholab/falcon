[Mesh]
  dim = 2
  file = square.e
[]

# Solves a pair of coupled diffusion equations where u=v on the boundary

[Variables]
  names = 'u v'

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
  names = 'diff_u diff_v'

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
  names = 'right_v left_u'

  [./right_v]
    type = DirichletBC
    variable = v
    boundary = 2
    value = 3
  [../]

  [./left_u]
    type = MatchedValueBC
    variable = u
    boundary = 1
    coupled_to = 'v'
    coupled_as = 'v'
    value = 3
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
  petsc_options = '-snes_mf_operator'

  nl_rel_tol = 1e-15
  l_tol = 1e-12
[]

[Output]
  file_base = out
  interval = 1
  exodus = true
[]
   
    
