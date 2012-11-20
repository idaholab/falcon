 [Mesh]
  type = MooseMesh
  file = simple_3d_May_2012.e
  []
  
[Variables]
  [./pressure]
  order = FIRST
  family = LAGRANGE
  initial_condition = 10e6
  [../]
  [./temperature]
  order = FIRST
  family = LAGRANGE
  initial_condition = 200.0
  [../]
  []

  
[Kernels]
  [./p_td]
  type = MassFluxTimeDerivative_PT
  variable = pressure
  [../]
  [./p_wmfp]
  type = WaterMassFluxPressure_PT
  variable = pressure
  [../]
  [./t_td]
  type = TemperatureTimeDerivative
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
  [./left_p]
  type = DirichletBC
  variable = pressure
  boundary = 1
  value = 10e6
  [../]
  [./left_t]
  type = DirichletBC
  variable = temperature
  boundary = 1
  value = 100.
  [../]
  [./right_p]
  type = DirichletBC
  variable = pressure
  boundary = 2
  value = 1e6
  [../]
  [./right_t]
  type = DirichletBC
  variable = temperature
  boundary = 2
  value = 200.
  [../]
  []
  
[Materials]
  [./rock]
  type = Geothermal
  block = 1
  pressure = pressure
  temperature = temperature
  water_steam_properties = water_steam_properties
  gravity = 0.0
  gx = 0.0
  gy = 0.0
  gz = 1.0
  material_porosity = 0.1
  permeability = 1.0e-15
  thermal_conductivity = 7.5E-5
  specific_heat_water = 4186E-6
  specific_heat_rock = 920E-6
  youngs_modulus = 1.50e10
  poissons_ratio = 0.3
  biot_coeff = 1.0
  thermal_expansion = 1.0e-6
  t_ref = 200
  [../]
  []
  
[UserObjects]
  [./water_steam_properties]
  type = WaterSteamEOS
  [../]
  []
  
[Executioner]
  type =  Transient
  num_steps = 5
  dt = 10.0
  nl_abs_tol = 1e-6

[./Quadrature]
  type = Trap
  [../]
  []
  
[Output]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
  []
  
