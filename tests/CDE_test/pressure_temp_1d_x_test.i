[Mesh]
  dim = 3
  file = 1d_x.e
[]
press
[Variables]
  active = 'pressure temperature'

   [./pressure]
    order = FIRST
    family = LAGRANGE
   initial_condition = 100000.0
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 20.0
   [../]
[]

[Kernels]
#  active = 'p_ie p_dmfp p_dmfz t_ie t_d t_c'
  active = 'p_dmfp t_d t_c'

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
  active = 'left_p right_p center_t left_t right_t'

  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 110000.0
  [../]

   [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 100000.0
  [../]

  [./center_t]
    type = DirichletBC
    variable = temperature
    boundary = 3
    value = 50.0
  [../]

  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 20.0
  [../]

  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 20.0
  [../]

[]


[Materials]
  active = 'PorousMedia'
#   active = 'DarcyWater'
  
  [./PorousMedia]
   type = Geothermal
   block = 1
    
    pressure = pressure
    temperature = temperature
#    x_disp = x_disp
#    y_disp = y_disp
#    z_disp = z_disp


    permeability         =   1.0e-12
    porosity             =   0.30
    rho_r                =   2.50e3
    rock_specific_heat   =   0.92e3
    thermal_conductivity =   2.5
    thermal_expansion    =   2.0e-6
    youngs_modulus       =   1.50e10
    poissons_ratio       =   0.3
    biot_coeff           =   0.2
    t_ref                =  200.0

    rho_w                = 1.0e03
    mu_w                 = 0.001
    c_f                  = 4.6e-10
    water_specific_heat  = 4.186e3

    temp_dependent_density = false
    has_solid_mechanics   = false

    gravity              = 0.0
  [../]

[ ]


[Executioner]
active = ' '	
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'
[]



[Output]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
[]
    
