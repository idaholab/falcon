[Mesh]
  dim = 3
  file = elder.e
[]

[Variables]
  active = 'pressure temperature v_x v_y v_z'

  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 100000.0
    scaling = 1e-6
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 20.0
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
#  active = 'p_dmfp t_d dv_x dv_y dv_z'

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
#  active = 'top_p top_t bot_t bot_t2 top_f bot_f bot_f2'

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
    value = 12.0
  [../]

 [./bot_t2]
    type = DirichletBC
    variable = temperature
    boundary = 4
    value = 12.0
  [../]

 [./top_f]
    type = GravityNeumannBC
    variable = pressure
    boundary = 1
    permeability = 1.e-10
    density = 1000
    mu_w = 0.001
  [../]

 [./bot_f]
    type = GravityNeumannBC
    variable = pressure
    boundary = 2
    permeability = 1.e-10
    density = 1000
    mu_w = 0.001
  [../]

  [./bot_f2]
    type = GravityNeumannBC
    variable = pressure
    boundary = 4
    permeability = 1.e-10
    density = 1000
    mu_w = 0.001
  [../]

[]

[Materials]
  active = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.e-10
    thermal_conductivity = 1.49
    rock_specific_heat = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = .1
    density = 1000.
    mu_w = .001
    gravity = 9.80655
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
#  petsc_options_iname = '-pc_type -sub_pc_type -ksp_gmres_restart'
#  petsc_options_value = 'asm lu 101'

  l_max_its = 60
  nl_rel_tol = 1e-12
[]

[Output]
  file_base = elder_IC_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
