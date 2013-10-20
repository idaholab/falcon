# file PH_EOS_STD_1D.i
# created 15 May 2013
# Rob Podgorney & Kat Smith, Idaho National Lab
# A simple one-dimensional, two-phase example problem demonstrating a basic use of the code.  A mass flux boundary (Neumann) condition is applied to the left end of the domain, and a constant pressure (Dirichlet) condition is applied on the right edge. The enthalpy boundaries mimic the pressure, with Neuman and Dirichlet conditions on the left and right.  The right conditions are such that the temperature is fixed at approxmately 277.8 degress C.

[Mesh]
  type = FileMesh
  file = 1d_col.e
[]

[Variables]
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 5e6
    scaling = 1e6
  [../]
  [./enthalpy]
    order = FIRST
    family = LAGRANGE
    initial_condition = 2850000
  [../]
[]

[AuxVariables]
  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./density_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./density_steam]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity_steam]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./enthalpy_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./enthalpy_steam]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./saturation_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./temperature]
    order = FIRST
    family = LAGRANGE
  [../]
  [./v_x]
    order = CONSTANT
    family = MONOMIAL
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
    temperature = temperature
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
  [./density]
    type = MaterialRealAux
    variable = density
    property = density
  [../]
  [./density_water]
    type = MaterialRealAux
    variable = density_water
    property = density_water
  [../]
  [./density_steam]
    type = MaterialRealAux
    variable = density_steam
    property = density_steam
  [../]
  [./enthalpy_water]
    type = MaterialRealAux
    variable = enthalpy_water
    property = enthalpy_water
  [../]
  [./enthalpy_steam]
    type = MaterialRealAux
    variable = enthalpy_steam
    property = enthalpy_steam
  [../]
  [./saturation_water]
    type = MaterialRealAux
    variable = saturation_water
    property = saturation_water
  [../]
  [./temperature]
    type = CoupledTemperatureAux
    variable = temperature
    pressure = pressure
    enthalpy = enthalpy
    water_steam_properties = water_steam_properties
  [../]
  [./viscosity_water]
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
  [../]
  [./viscosity_steam]
    type = MaterialRealAux
    variable = viscosity_steam
    property = viscosity_steam
  [../]
  [./v_x]
    type = VelocityAux
    variable = v_x
  [../]
[]

[BCs]
  [./left_p]
    type = NeumannBC
    variable = pressure
    boundary = 1
    value = 881.06645536150575e-7
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 5e6
  [../]
  [./left_t]
    type = NeumannBC
    variable = enthalpy
    boundary = 1
    value = 35.24266
  [../]
  [./right_t]
    type = DirichletBC
    variable = enthalpy
    boundary = 2
    value = 2850000
  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
  [../]
  [./dofs]
    type = NumDOFs
  [../]
[]

[Materials]
  active = 'GeothermalMaterial'
  [./GeothermalMaterial]
    block = 1
    water_steam_properties = water_steam_properties
    fluid_flow = true
    enthalpy = enthalpy
    pressure = pressure
    heat_transport = true
    solid_mechanics = false
    chemical_reactions = false
    temperature = temperature
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
  solve_type = PJFNK
  petsc_options_iname = ' -snes_ls -pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = ' basic  hypre boomeramg 201'
  l_max_its = 150
  nl_abs_tol = 1e-10
  nl_max_its = 60
  dt = 1.
  dtmax = 25000.0
  dtmin = 1E-1
  end_time = 864000
  nl_rel_tol = 1e-07

  [./Quadrature]
    type = Trap
  [../]
[]

[Output]
  file_base = PH_EOS_STD_1D_out
  output_initial = true
  exodus = true
[]

