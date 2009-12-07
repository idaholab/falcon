[Mesh]
  dim = 3
  file = 100Mcube.e
[]

[Variables]
  names = 'pressure'

  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 900000.0
   [../]
[]

[Kernels]
  names = 'p_dmfp p_dmfz'

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
[]

[BCs]
  names = 'left_p right_p'

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
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'

  [./Transient]
    start_time = 0.0
 #   end_time = 1.32e7
    num_steps = 10
    dt = 50000
    trans_ss_check = true
 #   ss_check_tol = 5.0e-5
    sol_time_adaptive_time_stepping = true
  [../]
[]

[Output]
  file_base = pressure_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
