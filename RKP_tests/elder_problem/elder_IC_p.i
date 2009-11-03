[Mesh]
  dim = 3
  file = elder.e
[]

[Variables]
  names = 'pressure temperature'

  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 100000.0
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 12.0
   [../]

[]
[Kernels]
  names = 'p_dmfp p_dmfz t_d t_c'

  [./p_dmfp]
    type = DarcyMassFluxPressure
    variable = pressure
  [../]

  [./p_dmfz]
    type = DarcyMassFluxZ
    variable = pressure
  [../]

  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
  [../]

  [./t_c]
    type = TemperatureConvection
    variable = temperature
  [../]

[]

[BCs]
  names = 'top_p top_t bot_t'

  [./top_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 100000.0
  [../]

 [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 12.0
  [../]

 [./bot_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 12.0
  [../]
[]

[Materials]
  names = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.21e-10
    thermal_conductivity = 1.49
    rock_specific_heat = 0.0
    gravity = -9.80655
    rho_w = 1000.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = .1
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'
[]

[Output]
  file_base = elder_IC_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
