[Mesh]
  dim = 2
  file = square.e
[]

[Variables]
  names = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  names = 'diff'

  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  names = 'left right'

  [./left]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0
  [../]

  [./right]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 1
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
[]

[Output]
  file_base = steady_out
  interval = 1
  exodus = true
[]
   
    
