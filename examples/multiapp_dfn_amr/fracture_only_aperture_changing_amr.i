# Cold water injection into one side of the fracture network, and production from the other side
injection_rate = 10 # kg/s

#dummy values
#injection coordinates
x_in=0 #-1000000  #58.8124
y_in=0 #-1000000  #0.50384
z_in=0 #-1000000  #74.7838
#production coordinates
x_out=0 #1000000  #101.705
y_out=0 #1000000  #160.459
z_out=0 #1000000  #39.5722


[Mesh]
  uniform_refine = 0
  [cluster34]
    type = FileMeshGenerator
    file = 'Cluster_34.exo'
  []
[]

[GlobalParams]
  PorousFlowDictator = dictator
  gravity = '0 0 -9.81E-6' # Note the value, because of pressure_unit
[]

[Variables]
  [frac_P]
    scaling = 1E6
  []
  [frac_T]
    initial_condition = 473
  []
[]

[ICs]
  [frac_P]
    type = FunctionIC
    variable = frac_P
    function = insitu_pp
  []
[]

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = frac_P
  temperature = frac_T
  fp = water
  pressure_unit = MPa
[]

[Kernels]
  [toMatrix]
    type = PorousFlowHeatMassTransfer
    variable = frac_T
    v = transferred_matrix_T
    transfer_coefficient = heat_transfer_coefficient
    save_in = joules_per_s
  []
[]

[AuxVariables]
  [heat_transfer_coefficient]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 0.0
  []
  [transferred_matrix_T]
    initial_condition = 473
  []
  [joules_per_s]
  []
  [normal_dirn_x]
    family = MONOMIAL
    order = CONSTANT
  []
  [normal_dirn_y]
    family = MONOMIAL
    order = CONSTANT
  []
  [normal_dirn_z]
    family = MONOMIAL
    order = CONSTANT
  []
  [enclosing_element_normal_length]
    family = MONOMIAL
    order = CONSTANT
  []
  [enclosing_element_normal_thermal_cond]
    family = MONOMIAL
    order = CONSTANT
  []
  [aperture]
    family = MONOMIAL
    order = CONSTANT
  []
  [perm_times_app]
    family = MONOMIAL
    order = CONSTANT
  []
  [density]
    family = MONOMIAL
    order = CONSTANT
  []
  [viscosity]
    family = MONOMIAL
    order = CONSTANT
  []
  [insitu_pp]
  []
  [frac_P_Pa]
    family = LAGRANGE
    order = FIRST
  []
[]

[AuxKernels]
  [normal_dirn_x_auxk]
    type = PorousFlowElementNormal
    variable = normal_dirn_x
    component = x
  []
  [normal_dirn_y]
    type = PorousFlowElementNormal
    variable = normal_dirn_y
    component = y
  []
  [normal_dirn_z]
    type = PorousFlowElementNormal
    variable = normal_dirn_z
    component = z
  []
  [heat_transfer_coefficient_auxk]
    type = ParsedAux
    variable = heat_transfer_coefficient
    args = 'enclosing_element_normal_length enclosing_element_normal_thermal_cond'
    constant_names = h_s
    constant_expressions = 1E3 # should be much bigger than thermal_conductivity / L ~ 1
    function = 'if(enclosing_element_normal_length = 0, 0, h_s * enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length / (h_s * enclosing_element_normal_length * enclosing_element_normal_length + enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length))'
  []
  [frac_P_Pa]
    type = ParsedAux
    variable = frac_P_Pa
    args = frac_P
    constant_names = MPa_to_Pa
    constant_expressions = 1E6
    function = 'frac_P*MPa_to_Pa'
  []
  [aperture]
    type = PorousFlowPropertyAux
    variable = aperture
    property = porosity
  []
  [perm_times_app]
    type = PorousFlowPropertyAux
    variable = perm_times_app
    property = permeability
    row = 0
    column = 0
  []
  [density]
    type = PorousFlowPropertyAux
    variable = density
    property = density
    phase = 0
  []
  [viscosity]
    type = PorousFlowPropertyAux
    variable = viscosity
    property = viscosity
    phase = 0
  []
  [insitu_pp]
    type = FunctionAux
    execute_on = initial
    variable = insitu_pp
    function = insitu_pp
  []
[]

[Reporters]
  [inject_pt]
    type=ConstantReporter
    real_vector_names = 'pt_x pt_y pt_z'
    real_vector_values = '${x_in}; ${y_in}; ${z_in}'
  []
  [inject_node]
    type=ClosestNode
    point_x = inject_pt/pt_x
    point_y = inject_pt/pt_y
    point_z = inject_pt/pt_z
    projection_tolerance = 100
    execute_on = TIMESTEP_BEGIN
    outputs = none
  []
  [TK_in]
    type=ClosestNodeData
    variable=frac_T
    point_x = inject_pt/pt_x
    point_y = inject_pt/pt_y
    point_z = inject_pt/pt_z
    projection_tolerance = 100
    execute_on = TIMESTEP_BEGIN
    outputs = none
  []
  [P_in]
    type=ClosestNodeData
    variable=frac_P
    point_x = inject_pt/pt_x
    point_y = inject_pt/pt_y
    point_z = inject_pt/pt_z
    projection_tolerance = 100
    execute_on = TIMESTEP_BEGIN
    outputs = none
  []
  [var_in]
    type = AccumulateReporter
    reporters = 'TK_in/node_id TK_in/node_x TK_in/node_y TK_in/node_z TK_in/frac_T P_in/frac_P'
    outputs = var_in
  []

  [prod_pt]
    type=ConstantReporter
    real_vector_names = 'pt_val pt_x pt_y pt_z'
    real_vector_values = '0.01; ${x_out}; ${y_out}; ${z_out}'
  []
  [prod_node]
    #production.xyz
    type=ClosestNodeProjector
    point_value =  prod_pt/pt_val
    point_x = prod_pt/pt_x
    point_y = prod_pt/pt_y
    point_z = prod_pt/pt_z
    projection_tolerance = 100
    execute_on = TIMESTEP_BEGIN
    outputs = none
  []

  [TK_out]
    type=ClosestNodeData
    variable=frac_T
    point_x = prod_pt/pt_x
    point_y = prod_pt/pt_y
    point_z = prod_pt/pt_z
    projection_tolerance = 100
    execute_on = TIMESTEP_BEGIN
    outputs = none
  []
  [P_out]
    type=ClosestNodeData
    variable=frac_P
    point_x = prod_pt/pt_x
    point_y = prod_pt/pt_y
    point_z = prod_pt/pt_z
    projection_tolerance = 100
    execute_on = TIMESTEP_BEGIN
    outputs = none
  []
  [var_out]
    type = AccumulateReporter
    reporters = 'TK_out/node_id TK_out/node_x TK_out/node_y TK_out/node_z TK_out/frac_T P_out/frac_P'
    outputs = var_out
  []
[]

[DiracKernels]
  [inject_fluid_mass]
    type = PorousFlowReporterPointSourcePP
    mass_flux = inject_mass_flux
    x_coord_reporter = 'inject_node/node_x'
    y_coord_reporter = 'inject_node/node_y'
    z_coord_reporter = 'inject_node/node_z'
    variable = frac_P
  []
  [inject_fluid_h]
    type = PorousFlowReporterPointEnthalpySourcePP
    variable = frac_T
    mass_flux = inject_mass_flux
    x_coord_reporter = 'inject_node/node_x'
    y_coord_reporter = 'inject_node/node_y'
    z_coord_reporter = 'inject_node/node_z'
    T_in = 'inject_T'
    pressure = frac_P_Pa
    fp = water
  []
  [withdraw_fluid]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = kg_out_uo
    bottom_p_or_t = insitu_pp_borehole
    character = 1
    line_length = 1
    x_coord_reporter = 'prod_node/node_x'
    y_coord_reporter = 'prod_node/node_y'
    z_coord_reporter = 'prod_node/node_z'
    weight_reporter = 'prod_node/node_value'
    unit_weight = '0 0 0'
    fluid_phase = 0
    use_mobility = true
    variable = frac_P
  []
  [withdraw_heat]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = J_out_uo
    bottom_p_or_t = insitu_pp_borehole
    character = 1
    line_length = 1
    x_coord_reporter = 'prod_node/node_x'
    y_coord_reporter = 'prod_node/node_y'
    z_coord_reporter = 'prod_node/node_z'
    weight_reporter = 'prod_node/node_value'
    unit_weight = '0 0 0'
    fluid_phase = 0
    use_mobility = true
    use_enthalpy = true
    variable = frac_T
  []
[]

[UserObjects]
  [kg_out_uo]
    type = PorousFlowSumQuantity
  []
  [J_out_uo]
    type = PorousFlowSumQuantity
  []
[]

[FluidProperties]
  [true_water]
    type = Water97FluidProperties
  []
  [water]
    type = TabulatedFluidProperties
    fp = true_water
    temperature_min = 275 # K
    temperature_max = 600
    interpolated_properties = 'density viscosity enthalpy internal_energy'
    fluid_property_file = water97_tabulated.csv
  []
[]

[Materials]
  [porosity]
    type = PorousFlowPorosityLinear
    porosity_ref = 1E-4 # fracture porosity = 1.0, but must include fracture aperture of 1E-4 at P = insitu_pp
    P_ref = insitu_pp
    P_coeff = 1E-3 # this is in metres/MPa, ie for P_ref = 1/P_coeff, the aperture becomes 1 metre
    porosity_min = 1E-5
  []
  [permeability]
    type = PorousFlowPermeabilityKozenyCarman
    k0 = 1E-15  # fracture perm = 1E-11 m^2, but must include fracture aperture of 1E-4
    poroperm_function = kozeny_carman_phi0
    m = 0
    n = 3
    phi0 = 1E-4
  []
  [internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2700 # kg/m^3
    specific_heat_capacity = 0 # basically no rock inside the fracture
  []
  [aq_thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '0.6E-4 0 0  0 0.6E-4 0  0 0 0.6E-4' # thermal conductivity of water times fracture aperture.  This should increase linearly with aperture, but is set constant in this model
  []
[]

[Functions]
  [kg_rate]
    type = ParsedFunction
    vals = 'dt kg_out'
    vars = 'dt kg_out'
    value = 'kg_out/dt'
  []
  [insitu_pp]
    type = ParsedFunction
    value = '10 - 0.847E-2 * z' # Approximate hydrostatic in MPa
  []
  # approx insitu at production point, to prevent aperture closing due to low porepressures
  [insitu_pp_borehole]
    type = ParsedFunction
    value = '10 - 0.847E-2 * z + 1' # Approximate hydrostatic in MPa + 1MPa
  []
[]

[Postprocessors]
  [inject_T]
    type = Receiver
    default = '373'
  []
  [inject_mass_flux]
    type = Receiver
    default = ${injection_rate}
  []
  [nl_its]
    type = NumNonlinearIterations
  []
  [total_nl_its]
    type = CumulativeValuePostprocessor
    postprocessor = nl_its
  []
  [l_its]
    type = NumLinearIterations
  []
  [total_l_its]
    type = CumulativeValuePostprocessor
    postprocessor = l_its
  []
  [dt]
    type = TimestepSize
  []
  [active_time]
    type = PerfGraphData
    section_name = "Root"
    data_type = total
  []
  [kg_out]
    type = PorousFlowPlotQuantity
    uo = kg_out_uo
  []
  [kg_per_s]
    type = FunctionValuePostprocessor
    function = kg_rate
  []
  [J_out]
    type = PorousFlowPlotQuantity
    uo = J_out_uo
  []
[]

[VectorPostprocessors]
  [heat_transfer_rate]
    type = NodalValueSampler
    outputs = none
    sort_by = id
    variable = joules_per_s
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
    dt = 0.3
    growth_factor = 1.1
    optimal_iterations = 6
  []

  # fixme Rob told me to try this.
  # The system doesn't change much once it is pressurized so the residual can't change much either
  steady_state_detection = true
  steady_state_start_time = 7000  #this should start after the system has pressurized
  steady_state_tolerance = 1e-5   #fixme should be smaller than nl_resid

  dtmin = 1e-3
  dtmax = 1000 #courant condition Peclet number (advection versus diffusion) limits dtmax
  end_time = 1e8
  line_search = 'none'
  automatic_scaling = false
  l_max_its = 20
  l_tol = 8e-3
  nl_forced_its = 1
  nl_max_its = 20
  nl_rel_tol = 5e-05
  nl_abs_tol = 1e-10
[]

[Outputs]
  print_linear_residuals = false
  #file_base = 'amr/frac'
  csv=false
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
  [var_in]
    type = JSON
    execute_system_information_on = none
    execute_on = 'FINAL'
    #file_base = 'var_in'
  []
  [var_out]
    type = JSON
    execute_system_information_on = none
    execute_on = 'FINAL'
    #file_base = 'var_out'
  []
[]
