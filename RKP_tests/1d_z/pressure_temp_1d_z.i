[Mesh]
  dim = 3
  file = pressure_1d_z_out.e
[]
press
[Variables]
  names = 'pressure temperature'

   [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_from_file = 'pressure 2'
 #   initial_condition = 900000.0
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10.0
   [../]
[]

[Kernels]
  names = 'p_ie p_dmfp p_dmfz t_ie t_d t_c'

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

  [./t_c]
    type = TemperatureConvection
    variable = temperature
  [../]

[]

[BCs]
  names = 'top_p center_t bottom_f top_t bottom_t'
#  names = 'left'

  [./top_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 10.0
  [../]
 
  [./bottom_f]
    type = NeumannBC
    variable = pressure
    boundary = 2
    value = -1.0
  [../]

  [./center_t]
    type = DirichletBC
    variable = temperature
    boundary = 3
    value = 50.0
  [../]

  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 10.0
  [../]

  [./bottom_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 10.0
  [../]

[]

[Materials]
  names = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure'
    coupled_as = 'pressure'
    permeability = 1.0E-08
    gravity = -9.80665
    rho_w = 1000.0
    gx = 0.0
    gy = 0.0
    gz = -1.0
    porosity = .1
    thermal_conductivity = 1.0
    water_specific_heat = 10.0
    rock_specific_heat = 10.0
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator'

  nl_rel_tol = 1e-10

  [./Transient]
    start_time = 0.0
 #   end_time = 1.32e7
     num_steps = 5
     dt = 1000
#    trans_ss_check = true
 #   ss_check_tol = 5.0e-5
    sol_time_adaptive_time_stepping = true
  [../]
[]

[Output]
  file_base = pressure_temp_1d_z_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
