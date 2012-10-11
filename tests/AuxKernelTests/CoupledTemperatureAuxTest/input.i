[Mesh]
  type = MooseMesh
  file = simple_3d_May_2012.e
[]

[Variables]
  [./pressure]
    initial_condition = 20e6
    block = 1
  [../]
  [./enthalpy]
    initial_condition = 850000
    block = 1
  [../]
[]

[Preconditioning]
  [./FDP]
    # off_diag_row    = 'pressure'
    # off_diag_column = 'enthalpy'
    type = FDP
    petsc_options = '-snes_mf_operator -ksp_monitor'
    petsc_options_iname = ' -pc_type -mat_fd_coloring_err -mat_fd_type'
    petsc_options_value = ' lu 1.0e-9 ds'
    full = true
  [../]
[]

[AuxVariables]
  [./temperature]
    block = 1
  [../]
[]

[Kernels]
  [./p_wmfp]
    type = WaterMassFluxPressure
    variable = pressure
    enthalpy = enthalpy
  [../]
  [./t_d]
    type = EnthalpyDiffusion
    variable = enthalpy
    pressure = pressure
    temperature = temperature
  [../]
  [./t_cw]
    type = EnthalpyConvectionWater
    variable = enthalpy
    pressure = pressure
  [../]
[]

[AuxKernels]
  [./temperature]
    type = CoupledTemperatureAux
    variable = temperature
    pressure = pressure
    enthalpy = enthalpy
    water_steam_properties = water_steam_properties
    block = 1
  [../]
[]

[BCs]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 20e6
  [../]
  [./left_t]
    type = DirichletBC
    variable = enthalpy
    value = 850000
    boundary = 1
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 19e6
  [../]
  [./right_t]
    type = DirichletBC
    variable = enthalpy
    value = 800000
    boundary = 2
  [../]
[]

[Materials]
  [./rock]
    type = Geothermal
    block = 1
    pressure = pressure
    enthalpy = enthalpy
    water_steam_properties = water_steam_properties
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    material_porosity = 0.1
    permeability = 1.0e-15
    density_rock = 2500
    thermal_conductivity = 7.5
    specific_heat_water = 4186
    specific_heat_rock = 920
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  type = Steady
[]

[Output]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

