#### The reference test case in the paper of Xia et al.
#### at 2016 Stanford Geothermal Workshop.
#### The PDF paper is uploaded separately.

#### This is the input file for the Production Run

############################################################
[Mesh]
  file = out3d_ini.e
[]
############################################################
[Functions]
  [./initial_T_distribution]
    type = ParsedFunction
    value = 493.15-0.065*z
  [../]
[]
############################################################
[Variables]
  [./P]
    initial_from_file_var = P
    initial_from_file_timestep = 2
  [../]
  [./T]
    [./InitialCondition]
      type = FunctionIC
      function = initial_T_distribution
    [../]
  [../]
[]
############################################################
[ICs]
[]
############################################################
[Preconditioning]
  [./SMP]
    #type = FDP
    type = SMP
    off_diag_row    = 'P'
    off_diag_column = 'T'
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

  [./T_energy_residual]
    type = PTEnergyResidual
    variable = T
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
    type = PresetBC
    variable = P
    boundary = '1006 1007 1008 1009 1010' # T = 203.75 deg. C
    value = 30e6
  [../]

  [./T_in]
    type = PresetBC
    variable = T
    value = 353.15
    boundary = '1001 1002 1003 1004 1005 1111'
  [../]

  [./T_out]
    type = PTEnergyOutFlowBC
    variable = T
    boundary = '1006 1007 1008 1009 1010'
  [../]
[]
############################################################
[Materials]
  [./matrix]
    type = PTGeothermal
    block = '4'

    #pressure = P
    temperature = T

    reference_pressure = 30e6

    fluid_property_formulation = 'wseos'
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

  [./fractures]
    type = PTGeothermal
    block = '3'

    pressure = P
    temperature = T

    fluid_property_formulation = 'wseos'
    stabilizer = 'supg'

    permeability         = 5e-12
    porosity             = 0.1
    compressibility      = 4.0e-10
    density_rock         = 2700
    density_water        = 1000
    viscosity_water      = 0.001
    specific_heat_rock   = 790
    specific_heat_water  = 4181
    thermal_conductivity = 1.5 #100 work |
    gravity              = 9.80665
    gravity_direction    = '0 0 0'

    constant_pressure_gradient = '0 0 0'
  [../]
[]
############################################################
[Executioner]
  type = Transient

  solve_type = 'NEWTON' # default = PJFNK | NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type
                         -ksp_gmres_restart -snes_ls
                         -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 cubic 0.7'

  #dt         = 1e1

  dtmin = 1
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 10
  [../]

  end_time   = 1e9
  num_steps  = 2 # set to 1000000 for complete simulation

  l_max_its  = 30
  l_tol      = 1e-3 # could use 1e-3 for faster stepping
  nl_max_its = 10
  nl_rel_tol = 1e-2 # could use 1e-3 for faster stepping
  nl_abs_tol = 1e-7
[]

############################################################
[Outputs]
  [./Exodus]
    type = Exodus
    file_base = out3d_run
    #use_problem_dimension = false
    interval = 100
  [../]
  [./CSV]
    type = CSV
    file_base = out3d_run
    interval = 10
  [../]
  [./CONSOLE]
    type = Console
    output_linear = true
    output_nonlinear = true
    execute_postprocessors_on = 'none'
    interval = 1
  [../]
[]
############################################################
[Postprocessors]
  [./DT]
    type = TimestepSize
  [../]

  [./P_1001]
    type = SideAverageValue
    variable = P
    boundary = '1001'
  [../]
  [./P_1002]
    type = SideAverageValue
    variable = P
    boundary = '1002'
  [../]
  [./P_1003]
    type = SideAverageValue
    variable = P
    boundary = '1003'
  [../]
  [./P_1004]
    type = SideAverageValue
    variable = P
    boundary = '1004'
  [../]
  [./P_1005]
    type = SideAverageValue
    variable = P
    boundary = '1005'
  [../]

  [./T_1006]
    type = SideAverageValue
    variable = T
    boundary = '1006'
  [../]
  [./T_1007]
    type = SideAverageValue
    variable = T
    boundary = '1007'
  [../]
  [./T_1008]
    type = SideAverageValue
    variable = T
    boundary = '1008'
  [../]
  [./T_1009]
    type = SideAverageValue
    variable = T
    boundary = '1009'
  [../]
  [./T_1010]
    type = SideAverageValue
    variable = T
    boundary = '1010'
  [../]

  [./MassRate_1006]
    type = PTMassSideFluxIntegral
    variable = P
    boundary = '1006'
  [../]
  [./MassRate_1007]
    type = PTMassSideFluxIntegral
    variable = P
    boundary = '1007'
  [../]
  [./MassRate_1008]
    type = PTMassSideFluxIntegral
    variable = P
    boundary = '1008'
  [../]
  [./MassRate_1009]
    type = PTMassSideFluxIntegral
    variable = P
    boundary = '1009'
  [../]
  [./MassRate_1010]
    type = PTMassSideFluxIntegral
    variable = P
    boundary = '1010'
  [../]
[]
############################################################
