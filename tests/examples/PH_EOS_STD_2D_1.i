# This problem needs some more work, but it is a good starting example for how to set-up a
# pressure-enthalpy (two-phase) problme and also use the source/sink kernel for injection/extraction.
# This is a steady state problem, but can easily be converted into a transient problem by toggling
# on pressure and enthalpy time kernels and changing the exocutioner block to transient.
# This problem is suppose to show a hot-high pressure reservoir with an extraction
# well in the center.

#The source/sink is setup as extracting fluid at 1 kg/s, and to keep the energy balance you
#have to extract enthalpy at the background enthalpy times the fluid extraction rate.  The problem
#too stable as the enthaply portion essentailly has nothing to work for (no real changes).

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 20
  ny = 20
  xmin = -100
  xmax = 100
  ymin = -100
  ymax = 100
  zmin = -10
  zmax = 10
[]

[Variables]
  [./pressure]
    initial_condition = 1e7
  [../]
  [./enthalpy]
    initial_condition = 1343300
  [../]
[]

[AuxVariables]
  [./v_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./density_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./v_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./temperature]
    initial_condition = 573.15
  [../]
[]

[Kernels]
  active = 'heat_sink water_sink pressure_diffusion enthalpy_diffusion enthalpy_convection_water'
  [./pressure_diffusion]
    type = WaterMassFluxPressure
    variable = pressure
    enthalpy = enthalpy
  [../]
  [./pressure_time]
    type = MassFluxTimeDerivative
    variable = pressure
    enthalpy = enthalpy
  [../]
  [./enthalpy_convection_water]
    type = EnthalpyConvectionWater
    variable = enthalpy
    pressure = pressure
  [../]
  [./enthalpy_diffusion]
    type = EnthalpyDiffusion
    variable = enthalpy
    pressure = pressure
    temperature = temperature
  [../]
  [./enthalpy_time]
    type = EnthalpyTimeDerivative
    variable = enthalpy
    pressure = pressure
  [../]
  [./water_sink]
    type = SourceSink
    variable = pressure
    value = '-1.0 -1.0 -1.0'     # kg/s rate
    point = '0.0 0.0 0.0'
    size = '20.0 20.0 20.0'
  [../]
  [./heat_sink]
    type = SourceSink
    variable = enthalpy
    value = '-1343300 -1343300 -1343300' # J/kg * kg/s from above
    point = '0.0 0.0 0.0'
    size = '20.0 20.0 20.0'
  [../]
[]

[AuxKernels]
  [./vx]
    type = VelocityAux
    variable = v_x
    component = 0
  [../]
  [./density_water]
    type = MaterialRealAux
    variable = density_water
    property = density_water
  [../]
  [./viscosity_water]
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
  [../]
  [./vy]
    type = VelocityAux
    variable = v_y
    component = 1
  [../]
  [./temperature]
    type = CoupledTemperatureAux
    variable = temperature
    water_steam_properties = water_steam_properties
    enthalpy = enthalpy
    pressure = pressure
    temperature = 573.15
  [../]
[]

[BCs]
  [./outside_pressure]
    type = DirichletBC
    variable = pressure
    boundary = '1 2 3 4'
    value = 1e7
  [../]
  [./outside_enthalpy]
    type = DirichletBC
    variable = enthalpy
    boundary = '1 2 3 4'
    value = 1343300
  [../]
[]


[Materials]
   [./GeothermalMaterial]
    block = 0
    solid_mechanics = false
    heat_transport = true
    fluid_flow = true
    chemical_reactions = false
    pressure = pressure
    water_steam_properties = water_steam_properties
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    permeability = 5e-14
    thermal_conductivity = 200
    enthalpy = enthalpy
    temperature = temperature
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  type = Steady
  l_max_its = 200
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 201 '
  line_search = basic
  nl_abs_tol = 1e-10
  l_tol = 1e-04

  [./Quadrature]
    type = TRAP

  [../]
[]

[Outputs]
  file_base = PH_EOS_STD_2D_1_out
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
