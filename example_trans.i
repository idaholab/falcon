[Mesh]
  dim = 3
  file = cube.e
#  uniform_refine = 1
[ ]

[Variables]
#  active = 'pressure temperature'
  active = 'pressure temperature v_x v_y v_z'

   [./pressure]
    order = FIRST
    family = LAGRANGE
   [../]
  [./temperature]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
        type = ConstantIC
    	value = 20.0
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

  [./v_z]
    order = FIRST
    family = LAGRANGE
   [../]

[ ]

[AuxVariables]
  active = 'density viscosity'

  [./density]
    order = FIRST
    family = LAGRANGE
  [../]

  [./viscosity]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
#  active = 'p_ie p_dmfp p_dmfz t_ie t_d t_c' 
  active = 'p_ie p_dmfp p_dmfz t_ie t_d t_c dv_x dv_y dv_z' 

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

 [./dv_z]
    type = DarcyVelocity
    variable = v_z
    component = 2
[ ]

[AuxKernels]
  active = 'density viscosity'

  [./density]
    variable = density
    type = CoupledDensityAux
    coupled_to = 'temperature'
    coupled_as = 'temperature'
  [../]

  [./viscosity]
    variable = viscosity
    type = CoupledViscosityAux
    coupled_to = 'temperature'
    coupled_as = 'temperature'
  [../]
[]

[BCs]
#  active = 'top_t bot_t left_t left_f right_f'
#  active = 'left_p right_p left_t'
   active = 'bot_t bot_p'
  [./bot_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 9.8e5
 [../]

 [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 0.0
 [../]

 [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 1.0
 [../]

 [./bot_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 100.0
 [../]

 [./left_f]
    type = NeumannBC
    variable = pressure
    boundary = 4
    value = 0.5
 [../]

 [./right_f]
    type = NeumannBC
    variable = pressure
    boundary = 3
    value = -0.5
 [../]

[ ]

[Materials]
  active = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.e-10
    c_f = 1.0e-12
    thermal_conductivity = 7.5e1
    rock_specific_heat =  10.0e4
    water_specific_heat = 1!0.0e3
    gravity = 9.8
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = .5
  [../]
[ ]

[Execution]
#  active = 'Transient Adaptivity'
  active = 'Transient'
  type =  Transient
  perf_log =  true
  petsc_options =  '-snes_mf_operator'
  petsc_options_iname =  '-pc_type -pc_hypre_type'
  petsc_options_value =  'hypre    boomeramg'

#  auto_scaling = true

  l_max_its =  100
  l_tol =  1e-6

  nl_max_its =  10
  nl_rel_tol =  1e-6

  [./Transient]
    start_time =  0.0
    num_steps =  10
    dt =  100
#    sol_time_adaptive_time_stepping =  true
  [../]

  [./Adaptivity]
    initial_adaptivity = 3
    error_estimator = KellyErrorEstimator
    refine_fraction = 0.20
    coarsen_fraction = 0.10
#    weight_names = 'urea'
#    weight_values = '1.0'
    max_h_level = 3
  [../]
[ ]

[Output]
  file_base = example_trans_out
  output_initial = true
  interval = 1
  exodus = true
[ ]
    
