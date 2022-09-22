# 3D matrix app doing thermo-hydro PorousFlow and receiving heat energy via a VectorPostprocessor from the 2D fracture App
intial_temperature=473
endTime = 1e6 #1e8
[Mesh]
  uniform_refine = 0
  [generate]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 11
    xmin = -10
    xmax = 210
    ny = 9
    ymin = -10
    ymax = 160
    nz = 11
    zmin = -10
    zmax = 210
  []
[]

[GlobalParams]
  PorousFlowDictator = dictator
[]

[Variables]
  [matrix_P]
    scaling = 1E6
  []
  [matrix_T]
    initial_condition = ${intial_temperature}
  []
[]

[ICs]
  [frac_P]
    type = FunctionIC
    variable = matrix_P
    function = insitu_pp
  []
[]

[Functions]
  [insitu_pp]
    type = ParsedFunction
    value = '10 - 0.847E-2 * z' # Approximate hydrostatic in MPa
  []
[]

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = matrix_P
  temperature = matrix_T
  fp = water
  gravity = '0 0 -9.81E-6' # Note the value, because of pressure_unit
  pressure_unit = MPa
[]

[DiracKernels]
  [heat_from_fracture]
    type = ReporterPointSource
    variable = matrix_T
    value_name = heat_transfer_rate/transferred_joules_per_s
    x_coord_name = heat_transfer_rate/x
    y_coord_name = heat_transfer_rate/y
    z_coord_name = heat_transfer_rate/z
  []
[]

[FluidProperties]
  [water]
    type = SimpleFluidProperties # this is largely irrelevant here since we care about heat conduction only
    thermal_expansion = 0 # to prevent depressurization as the reservoir is cooled
  []
[]

[Materials]
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 1E-3 # small porosity of rock
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1E-18 0 0   0 1E-18 0   0 0 1E-18'
  []
  [internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2700 # kg/m^3
    specific_heat_capacity = 800 # rough guess at specific heat capacity
  []
  [aq_thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '5 0 0  0 5 0  0 0 5'
  []
[]

[VectorPostprocessors]
  [heat_transfer_rate]
    type = ConstantVectorPostprocessor
    vector_names = 'transferred_joules_per_s x y z'
    value = '0; 0; 0; 0'
    outputs = none
  []
[]

[Reporters]
  [totalEnergy]
    type = StatisticsReporter
    reporters = 'heat_transfer_rate/transferred_joules_per_s'
    compute = 'sum'
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
  [T_elem]
    family = MONOMIAL
    order = CONSTANT
  []
  [increase_10deg]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [normal_thermal_conductivity_auxk]
    type = ConstantAux
    variable = normal_thermal_conductivity
    value = 5 # very simple in this case
  []
  [element_normal_length_auxk]
    type = PorousFlowElementLength
    variable = element_normal_length
    direction = 'fracture_normal_x fracture_normal_y fracture_normal_z'
  []
  [T_elem]
    type = SelfAux
    variable = T_elem
    v = matrix_T
  [../]
  [increase_10deg]
    type = ParsedAux
    variable = increase_10deg
    constant_names = To
    constant_expressions = ${intial_temperature}
    args = 'T_elem'
    function = 'if( (To-T_elem)>10,1,0)'
  []
[]

[Preconditioning]
  [./superlu]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'gmres lu superlu_dist'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 1
    growth_factor = 1.1
    optimal_iterations = 6
  []
  end_time = ${endTime}
  line_search = 'none'
  automatic_scaling = true
  l_max_its = 60
  l_tol = 8e-3
  nl_forced_its = 1
  nl_max_its = 40
  nl_rel_tol = 5e-05
  nl_abs_tol = 1e-10
[]

[Postprocessors]
  [volume_10deg]
    type = ElementIntegralVariablePostprocessor
    variable = increase_10deg
    #outputs = none
  []
  [nelems]
    type = NumElems
    outputs = none
  []
  [nl_its]
    type = NumNonlinearIterations
    outputs = none
  []
  [total_nl_its]
    type = CumulativeValuePostprocessor
    postprocessor = nl_its
    #outputs = none
  []
  [l_its]
    type = NumLinearIterations
    outputs = none
  []
  [total_l_its]
    type = CumulativeValuePostprocessor
    postprocessor = l_its
    #outputs = none
  []
  [dt]
    type = TimestepSize
    #outputs = none
  []
  [active_time]
    type = PerfGraphData
    section_name = "Root"
    data_type = total
    #outputs = none
  []
  [time]
    type = TimePostprocessor
    outputs = none
  []
[]


[Adaptivity]
  marker = points
  max_h_level = 1
  stop_time = 10
  [Markers]
    [points]
      type = ReporterPointMarker
      x_coord_name = heat_transfer_rate/x
      y_coord_name = heat_transfer_rate/y
      z_coord_name = heat_transfer_rate/z
      inside = refine
      empty = do_nothing
    []
  []
[]

#this is suppressing some output and
[Outputs]
  print_linear_residuals = false
  #file_base = 'amr2/matrix'
  csv=True
  exodus=False
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
    2700000 2800000 2900000 3e6 1e7 1e8'
    sync_only = true
  []
[]

[MultiApps]
  [fracture_app]
    type = TransientMultiApp
    input_files = fracture_only_aperture_changing_amr_orig.i
    #cli_args = 'Outputs/file_base=amr2/frac'
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
    source_variable = matrix_T
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
[]
