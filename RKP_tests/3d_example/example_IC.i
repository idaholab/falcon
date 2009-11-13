[Mesh]
  dim = 3
  file = 3d_10m.e
[]

[Variables]
  active = 'pressure temperature v_x v_y v_z'

  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 120000.0
    scaling = 1e-6
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 80.0
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
[]

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
  active = 'p_dmfp p_dmfz t_d dv_x dv_y dv_z'

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
  [../]
[]

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
  active = 'top_t bot_t left_p right_p'

  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 90.0
  [../]

 [./bot_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 90.0
  [../]

 [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 5
    value = 100000.0
  [../]

 [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 6
    value = 100000.0
  [../]
[]

[Materials]
  active = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.e-9
    thermal_conductivity = 1.5
    rock_specific_heat = 1000.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = .40
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 60
#  nl_rel_tol = 1e-10
[]

[Output]
  file_base = example_IC_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
