# Cold water injection into one side of the fracture network, and production from the other side
injection_rate = 0.1 # kg/s
endTime = 3e8
[Mesh]
  uniform_refine = 0
  [generate]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    xmin = -5
    xmax = 5
    ny = 50
    ymin = -50
    ymax = 50
  []
  [rotate]
    type = TransformGenerator
    input = generate
    transform = ROTATE
    vector_value = '90 90 90'
  []
  [injection_node]
    type = BoundingBoxNodeSetGenerator
    input = rotate
    bottom_left = '-0.05 -51 -5.1'
    top_right = '0.05  -49.9 5.1'
    new_boundary = injection_node
  []
[]

[GlobalParams]
  PorousFlowDictator = dictator
  gravity = '0 0 -9.81'
[]

[Variables]
  [frac_P]
  []
  [frac_T]
  []
[]

[ICs]
  [frac_P]
    type = FunctionIC
    variable = frac_P
    function = insitu_pp
  []
  [frac_T]
    type = FunctionIC
    variable = frac_T
    function = insitu_T
  []
[]

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = frac_P
  temperature = frac_T
  fp = the_simple_fluid #water
  pressure_unit = Pa
  # stabilization = full
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
  [transferred_matrix_T]
    initial_condition = 363
  []
  [heat_transfer_coefficient]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 0.0
  []
  [joules_per_s]
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
    coupled_variables = 'enclosing_element_normal_length enclosing_element_normal_thermal_cond'
    constant_names = h_s
    constant_expressions = 1E3 #This is the value being assigned to h_s.   Should be much bigger than thermal_conductivity / L ~ 1
    expression = 'if(enclosing_element_normal_length = 0, 0, h_s * enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length / (h_s * enclosing_element_normal_length * enclosing_element_normal_length + enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length))'
  []
  [insitu_pp]
    type = FunctionAux
    execute_on = initial
    variable = insitu_pp
    function = insitu_pp
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
[]

[BCs]
  [inject_heat]
    type = DirichletBC
    boundary = injection_node
    variable = frac_T
    value = 303
  []
[]

[DiracKernels]
  [inject_fluid]
    type = PorousFlowPointSourceFromPostprocessor
    mass_flux = ${injection_rate}
    point = '0 -50 0'
    variable = frac_P
  []
  [withdraw_fluid]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = kg_out_uo
    bottom_p_or_t = 30.6e6
    character = 1
    line_length = 1
    point_file = production_single_fracture.xyz
    unit_weight = '0 0 0'
    fluid_phase = 0
    use_mobility = true
    variable = frac_P
  []
  [withdraw_heat]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = J_out_uo
    bottom_p_or_t = 30.6e6
    character = 1
    line_length = 1
    point_file = production_single_fracture.xyz
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
  [the_simple_fluid]
    type = SimpleFluidProperties
    bulk_modulus = 2E9
    viscosity = 1.0E-3
    density0 = 1000.0
  []
  [true_water]
    type = Water97FluidProperties
  []
  [water]
    type = TabulatedBicubicFluidProperties
    fp = true_water
    temperature_min = 275 # K
    temperature_max = 600
    interpolated_properties = 'density viscosity enthalpy internal_energy'
    fluid_property_file = water97_tabulated.csv
  []
[]

[Materials]
  [porosity_frac]
    type = PorousFlowPorosityConst
    porosity = 0.01
  []
  [permeability_frac]
    type = PorousFlowPermeabilityConst
    permeability = '1e-12 0 0   0 1e-12 0   0 0 1e-12'
  []

  [internal_energy_frac]
    type = PorousFlowMatrixInternalEnergy
    density = 2700
    specific_heat_capacity = 0
  []
  [aq_thermal_conductivity_frac]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '0.6E-4 0 0  0 0.6E-4 0  0 0 0.6E-4'
  []
[]

[Functions]
  [kg_rate]
    type = ParsedFunction
    symbol_values = 'dt kg_out'
    symbol_names = 'dt kg_out'
    expression = 'kg_out/dt'
  []
  [insitu_pp]
    type = ParsedFunction
    expression = '9.81*1000*(3000 - z)'
  []
  [insitu_T]
    type = ParsedFunction
    expression = '363'
  []
[]

[Postprocessors]
  [dt]
    type = TimestepSize
    outputs = 'none'
  []
  [kg_out]
    type = PorousFlowPlotQuantity
    uo = kg_out_uo
    outputs = none
  []
  [kg_per_s]
    type = FunctionValuePostprocessor
    function = kg_rate
    outputs = none
  []
  [J_out]
    type = PorousFlowPlotQuantity
    uo = J_out_uo
    outputs = none
  []
  [TK_in]
    type = PointValue
    variable = frac_T
    point = '0 -50 0'
    outputs = none
  []
  [TK_out]
    type = PointValue
    variable = frac_T
    point = '0 50 0'
  []
  [P_out]
    type = PointValue
    variable = frac_P
    point = '0 50 0'
    outputs = none
  []
  [P_in]
    type = PointValue
    variable = frac_P
    point = '0 -50 0'
    outputs = none
  []
[]

[VectorPostprocessors]
  [heat_transfer_rate]
    type = NodalValueSampler
    sort_by = id
    variable = joules_per_s
    outputs = none
  []
[]

[Preconditioning]
  [superlu]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'gmres lu superlu_dist'
  []
[]

[Functions]
  [dts]
    type = ParsedFunction
    expression = if(t<1e6,t*t,1e6)
  []
[]
[Executioner]
  type = Transient
  solve_type = NEWTON
  # dt = 1e6
  dtmin = 1
  dtmax = 1e6
  end_time = ${endTime}
  line_search = 'none'
  automatic_scaling = true
  l_max_its = 20
  l_tol = 8e-3
  nl_forced_its = 1
  nl_max_its = 20
  nl_rel_tol = 5e-04
  nl_abs_tol = 1e-09
  [TimeStepper]
    type = FunctionDT
    function = dts
    min_dt = 2
  []
[]

[Outputs]
  print_linear_residuals = false
  csv = true
[]
