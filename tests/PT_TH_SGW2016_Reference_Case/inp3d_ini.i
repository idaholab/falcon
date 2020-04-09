#### The reference test case in the paper of Xia et al.
#### at 2016 Stanford Geothermal Workshop.
#### The PDF paper is uploaded separately.

#### This is the input file for the Initial Run

############################################################
[Mesh]
  file = mesh3d.e
[]
############################################################
[Variables]
  [./P]
    initial_condition = 30e6
  [../]
[]
############################################################
[AuxVariables]
  [./v_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./v_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./v_z]
    order = CONSTANT
    family = MONOMIAL
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
  [./vx]
    type = PTFluidVelocityAux
    variable = v_x
    component = 0
  [../]
  [./vy]
    type = PTFluidVelocityAux
    variable = v_y
    component = 1
  [../]
  [./vz]
    type = PTFluidVelocityAux
    variable = v_z
    component = 2
  [../]
[]
############################################################
[BCs]
  [./P_in]
    type = PTMassFluxBC
    variable = P
    boundary = '1001 1002 1003 1004 1005'
    value = 0.5
  [../]

  [./P_out]
    type = DirichletBC
    variable = P
    boundary = '1006 1007 1008 1009 1010'
    value = 30e6
  [../]
[]
############################################################
[Materials]
  [./fractures]
    type = PTGeothermal
    block = '3'

    pressure = P
    #temperature = T

    fluid_property_formulation = 'constant'
    stabilizer = 'none'

    permeability         = 5e-12
    porosity             = 0.1
    compressibility      = 4.0e-10
    density_rock         = 2700
    density_water        = 1000
    viscosity_water      = 0.001
    specific_heat_rock   = 790
    specific_heat_water  = 4181
    thermal_conductivity = 1.5
    gravity              = 9.80665
    gravity_direction    = '0 0 0'

    constant_pressure_gradient = '0 0 0'
  [../]

  [./matrix]
    type = PTGeothermal
    block = '4'

    pressure = P
    #temperature = T

    fluid_property_formulation = 'constant'
    stabilizer = 'none'

    permeability         = 1e-20
    porosity             = 0.01
    compressibility      = 4.0e-10
    density_rock         = 2700
    density_water        = 1000
    viscosity_water      = 0.001
    specific_heat_rock   = 790
    specific_heat_water  = 4181
    thermal_conductivity = 3
    gravity              = 9.80665
    gravity_direction    = '0 0 0'

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

  l_max_its = 400
  l_tol = 1e-3
  nl_max_its = 500
 #nl_rel_tol = 1e-5
  nl_abs_tol = 1e-6
[]

############################################################
[Outputs]
  #exodus = true
  [./Exodus]
    type = Exodus
    file_base = out3d_ini
    #use_problem_dimension = false
  [../]
  [./console]
    type = Console
    output_linear = true
    output_nonlinear = true
  [../]
[]
############################################################
