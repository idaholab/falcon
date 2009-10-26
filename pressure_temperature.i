[Mesh]
  dim = 3
  file = pressure_out.e
[]

[Variables]
  names = 'pressure temperature'

  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_from_file = 'pressure 2'
#    initial_condition = 900000.0
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 80.0
   [../]
[]

[Kernels]
  names = 'p_ie p_dmfp p_dmfz t_ie t_d'
#  names = 'p_ie p_dmfp'

  [./p_ie]
    type = DarcyImplicitEuler
    variable = pressure
  [../]

  [./p_dmfp]
    type = DarcyMassFluxPressure
    variable = pressure
  [../]

  [./p_dmfz]
    type = DarcyMassFluxZ
    variable = pressure
  [../]

  [./t_ie]
    type = TemperatureImplicitEuler
    variable = temperature
  [../]

  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
  [../]

[]

[BCs]
  names = 'left_p right_p edge_p left_t edge_t'
#  names = 'left'

  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 980665.0
  [../]

  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 882598.5
  [../]

  [./edge_p]
    type = NeumannBC
    variable = pressure
    boundary = 3
    value = 10.0
  [../]

 [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 80.0
  [../]

# [./right_t]
#    type = DirichletBC
#    variable = temperature
#    boundary = 2
#    value = 80.0
#  [../]

  [./edge_t]
    type = DirichletBC
    variable = temperature
    boundary = 3
    value = 50.0
  [../]
[]

[Materials]
  names = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure'
    coupled_as = 'pressure'
    permeability = 1.0E-8
    gravity = -9.80665
    rho_w = 999.43
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = 0.2
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator'

  [./Transient]
    start_time = 0.0
 #   end_time = 1.32e7
    num_steps = 500000
    dt = 50000
    trans_ss_check = true
 #   ss_check_tol = 5.0e-5
    sol_time_adaptive_time_stepping = true
  [../]
[]

[Output]
  file_base = pressure_temperature_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
