[Mesh]
  dim = 3
  file = elder_IC_out.e
[]

[Variables]
  active = 'pressure temperature'

   [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = 'pressure'
    initial_from_file_timestep = 2
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = 'temperature'
    initial_from_file_timestep = 2
   [../]
[]
[Kernels]
  active = 'p_ie p_dmfp p_dmfz t_ie t_d t_c'

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
  active = 'top_p top_t bot_t'

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
    value = 20.0
  [../]

[]

[Materials]
  active = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.21e-10
    thermal_conductivity = 0.149
    rock_specific_heat = 0.0
#    water_specific_heat = 1.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = .1
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator' 
  petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value =  'hypre    boomeramg 100'                           
  l_max_its =  20
#  l_tol =  1e-5                           
#  nl_max_its =  10                          
#  nl_rel_tol =  1e-5

  [./Transient]
    start_time = 0.0
#   end_time = 1.32e7
    num_steps = 24
# dt = 1 month
    dt = 259200.0
    trans_ss_check = true
    ss_check_tol = 5.0e-5
#   sol_time_adaptive_time_stepping = true
  [../]
[]

[Output]
  file_base = elder_trans_t_p_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
