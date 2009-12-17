[Mesh]
  dim = 3
  file = 1d_x_fine.e
[]

[Variables]
  active = 'pressure temperature v_x v_y v_z'

   [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 20
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
  active = 'density'

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
  active = 'p_ie p_dmfp t_ie t_d t_c dv_x dv_y dv_z' 

  [./p_ie]
    type = DarcyImplicitBackwardDifference2
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
    type = TemperatureImplicitBackwardDifference2
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
  active = 'density'

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
  active = 'left_p right_p left_t'

  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 10010.0
  [../]

   [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 10.0
  [../]

  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 30.0
  [../]

  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 10.0
  [../]
[]

[Materials]
  active = 'ThermalPoroElastic'
  
  [./ThermalPoroElastic]
    type = ThermalPoroElastic
    block = 1
    coupled_to = 'pressure temperature'
    coupled_as = 'pressure temperature'
    permeability = 1.0E-12
    gravity = 9.80665
    rho_w = 1000.
    rho_r = 2500.
    gx = 0.0
    gy = 0.0
    gz = 0.0
    porosity = .2
    thermal_conductivity = 2.5
    water_specific_heat = 4186.0
    rock_specific_heat = 920.0
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator' 
  petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value =  'hypre    boomeramg 100'                           
  
  l_max_its =  60
#  nl_rel_tol = 1e-10

  [./Transient]
    start_time = 0.0
 #   end_time = 1.32e7
     num_steps = 100
     dt = 10000.

#    trans_ss_check = true
 #   ss_check_tol = 5.0e-5
#    sol_time_adaptive_time_stepping = true
  [../]
[]

[Output]
  file_base = temperature_1d_fine_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
