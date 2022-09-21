#Example for Cornell diect use system
#R Podgorney
#2021.08.24

#part 2--transient simulation
endTime=63e6 #31.54e6

[Mesh]
  file = cornell_SS_out.e
[]

#############################################################
# [Adaptivity]
#     steps = 3
#     marker = box
#     [./Markers]
#       [./box]
#         bottom_left = '-200 1000 -2000'
#         inside = refine
#         top_right = '200 800 -2200'
#         outside = do_nothing
#         type = BoxMarker
#       [../]
#     [../]
# []



#############################################################
[GlobalParams]
  PorousFlowDictator = dictator
[]
############################################################
[Variables]
  [./P]
    initial_from_file_var = P
    initial_from_file_timestep = 'LATEST'
  [../]

  [./T]
    initial_from_file_var = T
    initial_from_file_timestep = 'LATEST'
    scaling = 1e-8
  [../]
[]
###########################################################
[BCs]
  [./pore_pressure_top]
    type = DirichletBC
    variable = P
    boundary = 5
    value = 20e6
  [../]

  [./temperature_top]
    type = DirichletBC
    variable = T
    boundary = 5
    value = 338.15
  [../]

  [./temperature_bot]
    type = DirichletBC
    variable = T
    boundary = 0
    value = 388.15
  [../]
[]
###########################################################
[AuxVariables]
  [./vel_x]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./vel_y]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./vel_z]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./viscosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]
###########################################################
[Kernels]
  [./P_time_deriv]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = P
  [../]

  [./energy_dot]
    type = PorousFlowEnergyTimeDerivative
    variable = T
  [../]

  [./P_flux]
    type = PorousFlowAdvectiveFlux
    fluid_component = 0
    variable = P
    gravity = '0 0 -9.8'
  [../]

  [./heat_conduction]
    type = PorousFlowHeatConduction
    variable = T
  [../]

  [./heat_advection]
    type = PorousFlowHeatAdvection
    variable = T
    gravity = '0 0 -9.8'
  [../]
[]
##############################################################
[AuxKernels]
  [./vel_x]
    type = PorousFlowDarcyVelocityComponent
    variable = vel_x
    component = x
    fluid_phase = 0
    gravity = '0 0 -9.8'
  [../]

  [./vel_y]
    type = PorousFlowDarcyVelocityComponent
    variable = vel_y
    component = y
    fluid_phase = 0
    gravity = '0 0 -9.8'
  [../]

  [./vel_z]
    type = PorousFlowDarcyVelocityComponent
    variable = vel_z
    component = z
    fluid_phase = 0
    gravity = '0 0 -9.8'
  [../]

  [./density]
    type = MaterialRealAux
     variable = density
     property = density
  [../]

  [./viscosity]
    type = MaterialRealAux
     variable = viscosity
     property = viscosity
  [../]
[]
############################################################
[UserObjects]
  [./dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'P T'
    number_fluid_phases = 1
    number_fluid_components = 1
  [../]

  [./pc]
    type = PorousFlowCapillaryPressureVG
    alpha = 1E-8
    m = 0.5
  [../]
[]
############################################################
[./FluidProperties]
  [./water]
    type = Water97FluidProperties
  [../]

  [./tabulated_water]
    type = TabulatedFluidProperties
    fp = water
    temperature_min = 275
    temperature_max = 450
    pressure_max = 1E8
    interpolated_properties = 'density viscosity enthalpy internal_energy'
    fluid_property_file = water97_tabulated.csv
  [../]
[../]
############################################################
[Materials]
  [./temperature]
    type = PorousFlowTemperature
    temperature = T
  [../]

  [./temperature_nodal]
    type = PorousFlowTemperature
    at_nodes = true
    temperature = T
  [../]

  [./ppss]
    type = PorousFlow1PhaseP
    at_nodes = true
    porepressure = P
    capillary_pressure = pc
  [../]

  [./ppss_qp]
    type = PorousFlow1PhaseP
    porepressure = P
    capillary_pressure = pc
  [../]

  [./massfrac]
    type = PorousFlowMassFraction
    at_nodes = true
  [../]

  [./fluid_props]
    type = PorousFlowSingleComponentFluid
    fp = water
    phase = 0
    at_nodes = true
  [../]

  [./fluid_props_qp]
    type = PorousFlowSingleComponentFluid
    phase = 0
    fp = water
  [../]

  [./rock_heat]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 825.0
    density = 2875.0
  [../]

  [./thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.83 0 0  0 2.83 0  0 0 2.83'
  [../]

  [./permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1.0E-16 0 0  0 1.0E-16 0  0 0 1.0E-16'
  [../]

  [./porosity_qp]
    type = PorousFlowPorosityConst
    porosity = 0.1
  [../]

  [./relperm]
    type = PorousFlowRelativePermeabilityCorey
    at_nodes = true
    n = 1
    phase = 0
  [../]

  [./relperm_qp]
    type = PorousFlowRelativePermeabilityCorey
    n = 1
    phase = 0
  [../]

  [./fp_mat]
    type = FluidPropertiesMaterialPT
    pressure = P
    temperature = T
    fp = water
  [../]
[]
############################################################
[Preconditioning]
  active = 'superlu'

  [./original]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type'
    petsc_options_value = 'gmres asm lu NONZERO'
  [../]

  [./superlu]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'gmres lu superlu_dist'
  [../]

  [./sub_pc_superlu]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type -sub_pc_factor_mat_solver_package'
    petsc_options_value = 'gmres asm lu NONZERO superlu_dist'
  [../]

  [./preferred]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
    petsc_options_value = ' lu       mumps'
  [../]
[]
############################################################
[Postprocessors]

[]
############################################################
[Executioner]
  type = Transient
  solve_type = NEWTON
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 1
    growth_factor = 1.1
    optimal_iterations = 6  #4  the timesteps are really small with 4  like 25s but eventually they get larger
    timestep_limiting_postprocessor = 1E8
  []
  end_time = ${endTime}

  # nl_abs_tol = 1E-2
  #fixme lynn I used all of the below solver options to get it to converge.
  #I couldn't get Andy's  "nl_abs_tol = 1E-2" to converge with the larger mesh
  #I am also using automatic scaling instead of the petsc_options in the preconditioner
  # line_search = 'none'
  # automatic_scaling = true
  l_max_its = 50
  l_tol = 1e-6
  nl_forced_its = 1
  nl_max_its = 10
  # nl_rel_tol = 5e-05
  nl_abs_tol = 1e-6
[]
############################################################
#fixme lynn this is all new for the transfer
[DiracKernels]
  [heat_from_fracture]
    type = ReporterPointSource
    variable = T
    value_name = heat_transfer_rate/transferred_joules_per_s
    x_coord_name = heat_transfer_rate/x
    y_coord_name = heat_transfer_rate/y
    z_coord_name = heat_transfer_rate/z
    # extra_vector_tags = 'ref'
  []
[]

[Reporters]
  [heat_transfer_rate]
    type = ConstantReporter
    real_vector_names = 'transferred_joules_per_s x y z'
    real_vector_values = '10000; 0; 0; 0'
    outputs = none
  []
[]

[AuxVariables]
  [normal_thermal_conductivity]
    family = MONOMIAL
    order = CONSTANT
  []
  [fracture_normal_x]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 0
  []
  [fracture_normal_y]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 1
  []
  [fracture_normal_z]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 0
  []
  [element_normal_length]
    family = MONOMIAL
    order = CONSTANT
  []
  [fracDensity_vizOnly]
  []
[]

[AuxKernels]
  [normal_thermal_conductivity_auxk]
    type = ConstantAux
    variable = normal_thermal_conductivity
    #fixme lynn this is very tuneable!
    value = 5 # very simple in this case
  []
  [element_normal_length_auxk]
    type = PorousFlowElementLength
    variable = element_normal_length
    direction = 'fracture_normal_x fracture_normal_y fracture_normal_z'
  []
[]

[Outputs]
  print_linear_residuals = false
  # exodus = false
  [matrix]
    type = Exodus
    sync_times = '1 10 100 200 300 400 500 600 700 800 900 1000 1100 1200 1300
    1400 1500 1600 1700 1800 1900 2000 2100 2200 2300 2400 2500 2600 2700 2800
    2900 3000 3100 3200 3300 3400 3500 3600 3700 3800 3900 4000 4100 4200 4300
    4400 4500 4600 4700 4800 4900 5000 5100 5200 5300 5400 5500 5600 5700 5800
    5900 6000 6100 6200 6300 6400 6500 6600 6700 6800 6900 7000 7100 7200 7300
    7400 7500 7600 7700 7800 7900 8000 8100 8200 8300 8400 8500 8600 8700 8800
    8900 9000 10000 11000 12000 13000 14000 15000 16000 17000 18000 19000 20000
    30000 50000 70000 100000 200000 300000 400000 500000 600000 700000 800000
    900000 1000000 1100000 1200000 1300000 1400000 1500000 1600000 1700000
    1800000 1900000 2000000 2100000 2200000 2300000 2400000 2500000 2600000
    2700000 2800000 2900000'
    sync_only = true
  []
[]

[MultiApps]
  [fracture_app]
    type = TransientMultiApp
    input_files = cornell_fracture_app.i
    cli_args = 'Outputs/frac/sync_only=false'
    execute_on = TIMESTEP_BEGIN
    sub_cycling = true
### catch_up = true
### max_catch_up_steps = 100
  []
[]

[Transfers]
  [element_normal_length_to_fracture]
    type = MultiAppNearestNodeTransfer
    direction = to_multiapp
    multi_app = fracture_app
    source_variable = element_normal_length
    variable = enclosing_element_normal_length
  []
  [element_normal_thermal_cond_to_fracture]
    type = MultiAppNearestNodeTransfer
    direction = to_multiapp
    multi_app = fracture_app
    source_variable = normal_thermal_conductivity
    variable = enclosing_element_normal_thermal_cond
  []
  [T_to_fracture]
    type = MultiAppInterpolationTransfer
    direction = to_multiapp
    multi_app = fracture_app
    source_variable = T
    variable = transferred_matrix_T
  []
  [normal_x_from_fracture]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = fracture_app
    source_variable = normal_dirn_x
    variable = fracture_normal_x
  []
  [normal_y_from_fracture]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = fracture_app
    source_variable = normal_dirn_y
    variable = fracture_normal_y
  []
  [normal_z_from_fracture]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = fracture_app
    source_variable = normal_dirn_z
    variable = fracture_normal_z
  []
  [heat_from_fracture]
    type = MultiAppReporterTransfer
    direction = from_multiapp
    multi_app = fracture_app
    from_reporters = 'heat_transfer_rate/joules_per_s heat_transfer_rate/x heat_transfer_rate/y heat_transfer_rate/z'
    to_reporters = 'heat_transfer_rate/transferred_joules_per_s heat_transfer_rate/x heat_transfer_rate/y heat_transfer_rate/z'
  []
  [from_sub_fracDensity_vizOnly]  #used for refinement.
    source_variable = density
    direction = from_multiapp
    variable = fracDensity_vizOnly
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture_app
  []
[]

[Adaptivity]
  marker = marker
  max_h_level = 2
  stop_time = 10  #refine over a couple of timesteps, needed to reach max refinement.
  [./Markers]
    [./marker]
      type = ValueThresholdMarker
      variable = fracDensity_vizOnly
      refine = 0.1   #anything with a frac density greater than zero, refine
    [../]
  [../]
[]
