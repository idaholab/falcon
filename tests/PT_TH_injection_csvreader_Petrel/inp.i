############################################################
[Mesh]
  file = example.e
[]
############################################################
[Variables]
  [./P]
    order = FIRST
    family = LAGRANGE
    initial_condition = 30.0e6 # [Pa]
  [../]

  [./T]
    order = FIRST
    family = LAGRANGE
    initial_condition = 483.15 # [K]
  [../]
[]
############################################################
[AuxVariables]
  [./permeability]
    order = FIRST
    family = LAGRANGE
  [../]

  [./porosity]
    order = FIRST
    family = LAGRANGE
  [../]

  [./rock_density]
    order = FIRST
    family = LAGRANGE
  [../]

  [./rock_specific_heat]
    order = FIRST
    family = LAGRANGE
  [../]

  [./thermal_conductivity]
    order = FIRST
    family = LAGRANGE
  [../]
[]
############################################################
[Kernels]
  [./P_time_derivative]
    type = PTMassTimeDerivative
    variable = P
  [../]

  [./P_mass_residual]
    type = PTMassResidual
    variable = P
  [../]

  [./T_time_derivative]
    type = PTEnergyTimeDerivative
    variable = T
  [../]

  [./T_mass_residual]
    type = PTEnergyResidual
    variable = T
  [../]
[]
############################################################
[DiracKernels]
  [./P_mass_source]
    type = PTMassPointSource
    variable = P
    schedule = '0.0'
    value = '0.1'
    point = '1.0 1.0 1.0'
  [../]

  [./T_energy_source]
    type = PTEnergyPointSource
    variable = T
    schedule = '0.0'
    value = '0.1'
    point = '1.0 1.0 1.0'
    injection_temperature = 363.15
  [../]
[]
############################################################
[AuxKernels]
  [./permeability]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = permeability
    csv_reader_object_name = 'csvreader'
    attribute_name = 'permeability'
  [../]

  [./porosity]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = porosity
    csv_reader_object_name = 'csvreader'
    attribute_name = 'porosity'
  [../]

  [./rock_density]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = rock_density
    csv_reader_object_name = 'csvreader'
    attribute_name = 'rock_density'
  [../]

  [./rock_specific_heat]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = rock_specific_heat
    csv_reader_object_name = 'csvreader'
    attribute_name = 'rock_specific_heat'
  [../]

  [./thermal_conductivity]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = thermal_conductivity
    csv_reader_object_name = 'csvreader'
    attribute_name = 'thermal_conductivity'
  [../]
[]
############################################################
[VectorPostprocessors]
  [./csvreader]
    type = CSVReader
    csv_file = example.csv
  [../]
[]
############################################################
[BCs]
  [./P_bcs]
    type = DirichletBC
    variable = P
    boundary = '1 2 3 4 5 6'
    value = 30.0e+06
  [../]

  [./T_bcs]
    type = DirichletBC
    variable = T
    boundary = '1 2 3 4 5 6'
    value = 483.15
  [../]
[]
############################################################
[Materials]
  [./porous]
    type = PTGeothermal
    block = 10

    pressure = P
    temperature = T

    fluid_property_formulation = 'wseos'
    stabilizer = 'zero'

    ### read properties from a CSV file
    csv_permeability = permeability
    csv_porosity = porosity
    csv_density_rock = rock_density
    csv_specific_heat_rock = rock_specific_heat
    csv_thermal_conductivity = thermal_conductivity

   #permeability         = 1.000e-12 # default = 1.000e-12
   #porosity             = 0.300e+00 # default = 0.300e+00
   #compressibility      = 1.000e-05 # default = 1.000e-05
   #density_rock         = 2.500e+03 # default = 2.500e+03
   #density_water        = 1.000e+03 # default = 1.000e+03
   #viscosity_water      = 1.200e-04 # default = 1.200e-04
   #specific_heat_rock   = 0.920e+03 # default = 0.920e+03
   #specific_heat_water  = 4.186e+03 # default = 4.186e+03
   #thermal_conductivity = 2.500e+00 # default = 2.500e+00
    gravity              = 0.000e+00 # default = 9.80665
   #gravity_direction    = '0 0 1'   # default = '0 0 1'

   #constant_pressure_gradient = '0 0 0'
  [../]
[]
############################################################
[Executioner]
  type = Transient
  num_steps = 2
  dt = 1000

  solve_type = 'PJFNK' # default = PJFNK | NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type
                         -ksp_gmres_restart -snes_ls
                         -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 cubic 0.7'

  l_max_its  = 20
  l_tol      = 1e-4
  nl_max_its = 50
  nl_rel_tol = 1e-4
  nl_abs_tol = 1e-7
[]
############################################################
[Outputs]
  file_base      = out
  exodus         = true
  [./Console]
    type = Console
    output_linear = true
    output_nonlinear = true
  [../]
[]
############################################################
