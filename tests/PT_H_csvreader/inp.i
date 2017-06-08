############################################################
[Mesh]
  file = example.e
[]
############################################################
[Variables]
  [./P]
    order = FIRST
    family = LAGRANGE
    initial_condition = 1.0
  [../]
[]
############################################################
[AuxVariables]
  [./mat1]
    order = FIRST
    family = LAGRANGE
  [../]

  [./mat2]
    order = FIRST
    family = LAGRANGE
  [../]

  [./mat3]
    order = FIRST
    family = LAGRANGE
  [../]

  [./mat4]
    order = FIRST
    family = LAGRANGE
  [../]

  [./mat5]
    order = FIRST
    family = LAGRANGE
  [../]
[]
############################################################
[Kernels]
  [./P_mass_residual]
    type = PTMassResidual
    variable = P
  [../]
[]
############################################################
[AuxKernels]
  [./mat1]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = mat1
    csv_reader_object_name = 'csvreader'
    attribute_name = 'mat1'
  [../]

  [./mat2]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = mat2
    csv_reader_object_name = 'csvreader'
    attribute_name = 'mat2'
  [../]

  [./mat3]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = mat3
    csv_reader_object_name = 'csvreader'
    attribute_name = 'mat3'
  [../]

  [./mat4]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = mat4
    csv_reader_object_name = 'csvreader'
    attribute_name = 'mat4'
  [../]

  [./mat5]
    type = CSVNodalMaterialDataAux
    execute_on = 'initial'
    variable = mat5
    csv_reader_object_name = 'csvreader'
    attribute_name = 'mat5'
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
  [./P_left]
    type = PresetBC
    variable = P
    boundary = '1'
    value = 2.0
  [../]

  [./P_right]
    type = PresetBC
    variable = P
    boundary = '2'
    value = 1.0
  [../]
[]
############################################################
[Materials]
  [./porous]
    type = PTGeothermal
    block = 10

    pressure = P
    #temperature = T

    fluid_property_formulation = 'constant'
    stabilizer = 'none'

    permeability         = 1.000e-00 # default = 1.000e-12
    porosity             = 1.000e+00 # default = 0.300e+00
    compressibility      = 1.000e-00 # default = 1.000e-05
    density_rock         = 2.500e+03 # default = 2.500e+03
    density_water        = 1.000e+00 # default = 1.000e+03
    viscosity_water      = 1.000e-00 # default = 1.200e-04
    specific_heat_rock   = 0.920e+03 # default = 0.920e+03
    specific_heat_water  = 4.186e+03 # default = 4.186e+03
    thermal_conductivity = 2.500e+00 # default = 2.500e+00
    gravity              = 0.000e+00 # default = 9.80665
    gravity_direction    = '0 0 1'   # default = '0 0 1'

    constant_pressure_gradient = '0 0 0'
  [../]
[]
############################################################
[Executioner]
  type = Steady

  solve_type = 'NEWTON' # default = PJFNK | NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type
                         -ksp_gmres_restart -snes_ls
                         -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 cubic 0.7'

  l_max_its  = 400
  l_tol      = 1e-12
  nl_max_its = 15
  nl_rel_tol = 1e-5
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
