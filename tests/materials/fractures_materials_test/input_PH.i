# Testing the FracturesGeothermal material with a simple fluid flow and
# heat transport problem.  A very course mesh and fracture distribution is
# used to make the test run faster.  Constant pressure gradient from top
# to bottom, initial enthalpy (850000 J) corresponding to a temp of
# 100 C with a constant BC of ~53 C (650000 J) is applied to the
# top of the domain.  Temp front convects faster through the fracture.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 5
  ny = 14
  xmax = 5
  ymax = 14
[]

[Variables]
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10e6
  [../]
  [./enthalpy]
    initial_condition = 850000
  [../]
[]

[AuxVariables]
  [./fractures]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./temperature]
    order = FIRST
    family = LAGRANGE
  [../]
  [./saturation]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./p_wmfp]
    type = WaterMassFluxPressure
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
[]

[AuxKernels]
  [./fractures]
    type = StochasticFieldAux
    variable = fractures
    file_name = fractures_small.txt
  [../]
  [./temperature]
    type = CoupledTemperatureAux
    variable = temperature
    pressure = pressure
    enthalpy = enthalpy
    water_steam_properties = water_steam_properties
    temperature = 473.15
  [../]
  [./saturation]
    type = MaterialRealAux
    variable = saturation
    property = saturation_water
  [../]
[]

[BCs]
  [./top_p]
    type = DirichletBC
    variable = pressure
    boundary = top
    value = 10e6
  [../]
  [./bottom_p]
    type = DirichletBC
    variable = pressure
    boundary = bottom
    value = 9.9e6
  [../]
  [./bottom_t]
    type = DirichletBC
    variable = enthalpy
    boundary = bottom
    value = 850000
  [../]
  [./top_t]
    type = DirichletBC
    variable = enthalpy
    boundary = top
    value = 650000
  [../]
[]

[Materials]
  [./rock]
    type = FracturesGeothermal
    block = 0
    pressure = pressure
    enthalpy = enthalpy
    water_steam_properties = water_steam_properties
    fractures = fractures
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    fracture_density = 1000
    fracture_porosity = 0.90
    fracture_permeability = 1e-11
    fracture_thermal_expansion = 1e-6
    fracture_youngs_modulus = 1.5e+7
    fracture_thermal_conductivity = 100
    matrix_permeability = 1e-17
    matrix_porosity = 0.01
    matrix_thermal_expansion = 2e-05
    matrix_thermal_conductivity = 2.0
    temp_dependent_fluid_props = false
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  # nl_abs_tol = 1e-6
  type = Transient
  num_steps = 5
  dt = 100000.0

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  [./Quadrature]
    type = Trap
  [../]
[]

[Output]
  linear_residuals = true
  file_base = out_PH
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

