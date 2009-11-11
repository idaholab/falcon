[Mesh]
  dim = 3
  file = 1d_z.e
[]

[Variables]
  active = 'pressure temperature v_x v_y v_z'

   [./pressure]
    order = FIRST
    family = LAGRANGE
 #   initial_from_file = 'pressure 2'
    initial_condition = 90000.0
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 25.0
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
  active = 'top_p center_t top_t bottom_t bottom_f'
#  active = 'left'

  [./top_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 90000.0
  [../]
 

  [./center_t]
    type = DirichletBC
    variable = temperature
    boundary = 3
    value = 35.0
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

  [./bottom_f]
    type = NeumannBC
    variable = pressure
    boundary = 2
    value = 0
  [../]

[]

[Materials]
  active = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.0E-10
    gravity = 9.80665
    rho_w = 1000.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = .1
    thermal_conductivity = 1.0
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'

  nl_rel_tol = 1e-12

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
    
