############################################################
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmin = -100
  xmax = 100
  ymin = -100
  ymax = 100
[]
############################################################
[Variables]
  [./P]
    initial_condition = 0.10e6
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
[]
############################################################
[DiracKernels]
  [./P_mass_source]
    type = PTMassPointSource
    variable = P
    schedule = '0.0'
    value = '1'
    point = '0.0 0.0 0.0'
  [../]
[]
############################################################
[BCs]
  [./P_bcs]
    type = DirichletBC
    variable = P
    boundary = 'left right bottom top'
    value = 0.10e+06
  [../]
[]
############################################################
[Materials]
  [./porous]
    type = PTGeothermal
    block = 0

    pressure = P
    #temperature = T

    fluid_property_formulation = 'constant'
    stabilizer = 'none'

    permeability         = 1.0e-12   # default = 1.000e-12
    porosity             = 0.5       # default = 0.300e+00
   #compressibility      = 1.0       # default = 1.000e-05
    density_rock         = 2500      # default = 2.500e+03
    density_water        = 1000      # default = 1.000e+03
    viscosity_water      = 1.2e-4    # default = 1.200e-04
    specific_heat_rock   = 920       # default = 0.920e+03
    specific_heat_water  = 4186      # default = 4.186e+03
    thermal_conductivity = 2.5       # default = 2.500e+00
    gravity              = 0         # default = 9.80665
    gravity_direction    = '0 0 -1'   # default = '0 0 -1'

    constant_pressure_gradient = '0 0 0'
  [../]
[]
############################################################
[Executioner]
  type = Transient
  num_steps = 5
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
