[Mesh]
  dim = 3
  file = 1d_x.e
[]

[Variables]
  active = 'pressure'

  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10.0
   [../]
[]

[Kernels]
  active = 'p_dmfp p_dmfz'

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
  active = 'left_p right_p'

  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 1010.0
  [../]

   [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 10.0
  [../]

[Materials]
  active = 'darcy_water'
  
  [./darcy_water]
    type = DarcyWater
    block = 1
    coupled_to = 'pressure'
    coupled_as = 'pressure'
    permeability = 1.0E-06
    gravity = -9.80655
    rho_w = 1000.0
    gx = 0.0
    gy = 0.0
    gz = 0.0
    porosity = .1
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'

  [./Transient]
    start_time = 0.0
 #   end_time = 1.32e7
    num_steps = 500000
    dt = 50000
    trans_ss_check = true
    ss_check_tol = 5.0e-6
    sol_time_adaptive_time_stepping = true
  [../]
[]

[Output]
  file_base = pressure_1d_x_out
  output_initial = true
  interval = 1
  exodus = true
[]
    
