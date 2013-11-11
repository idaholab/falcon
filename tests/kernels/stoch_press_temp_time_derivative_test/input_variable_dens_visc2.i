[Mesh]
  type = FileMesh
  file = gold/out_variable_dens_visc1.e
[]

[Variables]
  [./pressure]
    initial_from_file_var = pressure
    initial_from_file_timestep = 6
  [../]
  [./temperature]
    initial_from_file_var = temperature
    initial_from_file_timestep = 6
  [../]
[]

[AuxVariables]
  [./permeability]
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

[AuxKernels]
  [./aux_permeability]
    type = StochasticFieldAux
    variable = permeability
    file_name = IESE_Permeability.txt
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
[]

[BCs]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 10e6
  [../]
  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 373.15
  [../]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 1e6
  [../]
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 473.15
  [../]
[]

[Materials]
active = 'rock'
  [./StochasticGeothermalMaterial]
    block = 1
    solid_mechanics = false
    heat_transport = true
    fluid_flow = true
    chemical_reactions = false
    pressure = pressure
    temperature = temperature
    permeability = permeability
    temp_dependent_fluid_props = true
    water_steam_properties = water_steam_properties
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0 
  [../]

  [./rock]
    type = StochasticGeothermal
    block = 1
    pressure = pressure
    temperature = temperature
    temp_dependent_fluid_props = true
    water_steam_properties = water_steam_properties
    permeability = permeability
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 2
  dt = 1000000.0

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  nl_abs_tol = 1e-6
  [./Quadrature]
    type = Trap
  [../]
[]

[Output]
  linear_residuals = true
  file_base = out_variable_dens_visc2
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

