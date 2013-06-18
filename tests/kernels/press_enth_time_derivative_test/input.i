[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  xmax = 10
[]

[Variables]
  [./pressure]
    initial_condition = 20e6
  [../]
  [./enthalpy]
    initial_condition = 850000
  [../]
[]

[AuxVariables]
  [./temperature]
  [../]
[]

[Kernels]
  [./p_td]
    type = MassFluxTimeDerivative
    variable = pressure
    enthalpy = enthalpy
  [../]
  [./p_wmfp]
    type = WaterMassFluxPressure
    variable = pressure
    enthalpy = enthalpy
  [../]
  [./p_wsfp]
    type = SteamMassFluxPressure
    variable = pressure
    enthalpy = enthalpy
  [../]
  [./t_td]
    type = EnthalpyTimeDerivative
    variable = enthalpy
    pressure = pressure
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
  [./t_cs]
    type = EnthalpyConvectionSteam
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
  [../]
[]

[BCs]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 20e6
  [../]
  [./left_t]
    type = DirichletBC
    variable = enthalpy
    value = 850000
    boundary = left
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 19e6
  [../]
  [./right_t]
    type = DirichletBC
    variable = enthalpy
    value = 800000
    boundary = right
  [../]
[]

[Materials]
  [./rock]
    type = Geothermal
    block = 0
    pressure = pressure
    enthalpy = enthalpy
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

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  dt = 1000000
  num_steps = 2
  petsc_options = -snes_mf_operator
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

