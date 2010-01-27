[Mesh]
  dim = 2
#  file = elder_IC_2d_10m_out.e
  file = elder_2d_10m.e
#  uniform_refine = 1
[ ]

[Variables]
  active = 'pressure temperature v_x v_y'

   [./pressure]
    order = FIRST
    family = LAGRANGE
#    initial_from_file_var = 'pressure'
#    initial_from_file_timestep = 2
     initial_condition = 100000
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
#    initial_from_file_var = 'temperature'
#    initial_from_file_timestep = 2
     [./InitialCondition]
       type = BoundingBoxIC
       inside = 20
       outside = 12
       x1 = 0
       y1 = -150
       x2 = 150
       y2 = -149.8
     [../]
   [../]

  [./v_x]
    order = FIRST
    family = LAGRANGE
   [../]

  [./v_y]
    order = FIRST
    family = LAGRANGE
   [../]
[ ]

[Kernels]
#  active = 'p_ie p_dmfp p_dmfz t_ie t_d t_c dv_x dv_y'
  active = 'p_dmfp p_dmfz  t_d dv_x dv_y'

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

  [./dv_x]
    type = DarcyVelocity
    variable = v_x
    component = 0
  [../]

  [./dv_y]
    type = DarcyVelocity
    variable = v_y
    component = 1
  [../]
[]

[BCs]
  active = 'top_p bot_t bot_t2'

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

 [./bot_t2]
    type = DirichletBC
    variable = temperature
    boundary = 3
    value = 12.0
  [../]

 [./bot_t3]
    type = DirichletBC
    variable = temperature
    boundary = 4
    value = 20.0
  [../]

[ ]

[Materials]
  active = 'ThermalPoroElastic'
  
  [./ThermalPoroElastic]
    type = ThermalPoroElastic
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.0E-10
    gravity = 9.80665
    rho_w = 1000.
    mu_w = 0.001
    rho_r = 2500.
    gx = 0.0
    gy = 1.0
    gz = 0.0
    porosity = .40
    thermal_conductivity = 1.0
    water_specific_heat = 4186.0
    rock_specific_heat = 920.0
  [../]
[ ]

[Execution]

#  active = 'Transient Adaptivity'
#  type = Transient
  active = 'Steady Adaptivity'
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator -ksp_monitor' 
  petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value =  'hypre    boomeramg 100'                           
  l_max_its = 60
  nl_rel_tol = 1e-8

  [./Transient]
    start_time = 0.0
#   end_time = 1.32e7
    num_steps = 1
    dt = 1.72e6
#    trans_ss_check = true
#    ss_check_tol = 1.0e-7
#   sol_time_adaptive_time_stepping = true
  [../]

  [./Adaptivity]
    initial_adaptivity = 0
    error_estimator = KellyErrorEstimator
    refine_fraction = 0.90
    coarsen_fraction = 0.01
    max_h_level = 5
#    weight_names = temperature
#    weight_values = 1
    steps = 4
  [../]
[ ]

[Output]
  file_base = steady_no_amr
  output_initial = true
  interval = 1
  exodus = true
[ ]
    
