[Mesh]
  dim = 3
  file = cube.e
[]

[Variables]
  names = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  names = 'heat ie'

  [./heat]
    type = HeatConduction
    variable = u
  [../]

  [./ie]
    type = HeatConductionImplicitEuler
    variable = u
  [../]
[]

[BCs]
  names = 'bottom top'

  [./bottom]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0.0
  [../]

  [./top]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 1.0
  [../]
[]

[Materials]
  names = 'constant'
  
  [./constant]
    type = Constant
    block = 1
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator'

  [./Transient]
    start_time = 0.0
    num_steps = 5
    dt = .1
  [../]
[]

[Output]
  file_base = out
  interval = 1
  exodus = true
[]
    
