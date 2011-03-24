[Mesh]
  dim = 3
  file = cube.e
[]

[Variables]
  active = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  active = 'heat ie'

  [./heat]
   # type = HeatConduction
    type = TemperatureDiffusion
    variable = u
  [../]

  [./ie]
   # type =  HeatConductionImplicitEuler
    type =  TemperatureImplicitEuler
    variable = u
  [../]
[]

[BCs]
  active = 'bottom top'

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
  active = 'PorousMedia'
  
  [./PorousMedia]
    type = 'Geothermal'
    block = 1
    density_rock         =   1.0
    specific_heat_rock   =   1.0
    thermal_conductivity = 1.0
    porosity = 0


  [../]
[]

[Executioner]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator'

  start_time = 0.0
  num_steps = 5
  dt = .1
[]

[Output]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
[]
    
