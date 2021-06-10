############################################################
[Mesh]
  type = GeneratedMesh
  dim  = 2
  nx   = 100
  ny   = 1
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 10.0
[]
############################################################
[Variables]
  [./T]
    order = FIRST
    family = LAGRANGE
    initial_condition = 2.0
  [../]
[]
############################################################
[Kernels]
  [./T_time_derivative]
    type = PTEnergyTimeDerivative
    variable = T
  [../]

  [./T_energy_residual]
    type = PTEnergyResidual
    variable = T
  [../]
[]
############################################################
[BCs]
  [./T_left]
    type = PTEnergyInFlowBC
    variable = T
    boundary = 'left'
    value = 1.0
  [../]

  [./T_right]
    type = PTEnergyOutFlowBC
    variable = T
    boundary = 'right'
  [../]
[]
############################################################
[Materials]
  [./porous]
    type = PTGeothermal
    block = 0

    #pressure = P
    temperature = T

    fluid_property_formulation = 'constant'
    stabilizer = 'supg'

    permeability         = 1.000e-00 # default = 1.000e-12
    porosity             = 1.000e+00 # default = 0.300e+00
    compressibility      = 1.000e-00 # default = 1.000e-05
    density_rock         = 2.500e+03 # default = 2.500e+03
    density_water        = 1.000e+00 # default = 1.000e+03
    viscosity_water      = 1.000e-00 # default = 1.200e-04
    specific_heat_rock   = 1.000e+00 # default = 0.920e+03
    specific_heat_water  = 1.000e+00 # default = 4.186e+03
    thermal_conductivity = 0.000e+00 # default = 2.500e+00
    gravity              = 0.000e+00 # default = 9.80665
    gravity_direction    = '0 0 1'   # default = '0 0 1'

    constant_pressure_gradient = '-1 0 0'
  [../]
[]
############################################################
[Executioner]
  type = Transient
  scheme = bdf2
  dt = 0.002
  num_steps = 250

  solve_type = 'PJFNK' # default = PJFNK | NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type
                         -ksp_gmres_restart -snes_ls
                         -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 cubic 0.7'

  l_max_its  = 50
  l_tol      = 1e-8
  nl_max_its = 15
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
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
[Postprocessors]
  [./SideProductionIntegral]
    type = PTEnergySideProductionIntegral
    variable = T
    boundary = 'right'
    injection_temperature = 1.0
    execute_on = 'initial timestep_end'
  [../]
[]
############################################################
