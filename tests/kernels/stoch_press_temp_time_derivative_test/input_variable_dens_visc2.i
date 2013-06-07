[Mesh]
  type = FileMesh
  file = gold/out_variable_dens_visc1.e
[]

[Variables]
  [./pressure]
    initial_from_file_var = pressure
    initial_from_file_timestep = 21
  [../]
  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = temperature
    initial_from_file_timestep = 21
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
    value = 100.
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
    value = 200.
  [../]
[]

[Materials]
  [./rock]
    type = StochasticGeothermal
    block = 1
    pressure = pressure
    temperature = temperature
    water_steam_properties = water_steam_properties
    permeability = permeability
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    youngs_modulus = 1.50e10
    poissons_ratio = 0.3
    biot_coeff = 1.0
    thermal_expansion = 1.0e-6
    t_ref = 50
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
  petsc_options = '-ksp_monitor -snes_mf_operator'
  nl_abs_tol = 1e-6
  [./Quadrature]
    type = Trap
  [../]
[]

[Output]
  file_base = out_variable_dens_visc2
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

