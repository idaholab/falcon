[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  xmax = 10
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
    initial_condition = 473.15
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
    boundary = left
    value = 10e6
  [../]
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 373.15
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 1e6
  [../]
  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 473.15
  [../]
[]

[Materials]
  [./rock]
    type = Geothermal
    block = 0
    pressure = pressure
    temperature = temperature
    water_steam_properties = water_steam_properties
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = 0.1
    permeability = 1.0e-15
    thermal_conductivity = 7.5  
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 5
  dt = 10.0
  petsc_options = '-snes_mf_operator -ksp_monitor'
  nl_abs_tol = 1e-6
  [./Quadrature]
    type = Trap
  [../]
[]

[Output]
  file_base = out_PT_time_derivative_w_diamond_material
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

