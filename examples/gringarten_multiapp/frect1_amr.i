# Cold water injection into one side of the fracture network, and production from the other side
injection_rate = 0.1 #25 # kg/s
endTime = 3.16e8


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
  [./rotate]
    type = TransformGenerator
    input = generate
    transform = ROTATE
    vector_value = '90 90 90'
  []
  [./offset]
    type = TransformGenerator
    input = rotate
    transform = TRANSLATE
    vector_value = '0.1 0 0'
  []
  [injection_node]
    type = BoundingBoxNodeSetGenerator
    input = offset
    bottom_left = '0.05 -51 -5.1'
    top_right = '0.15  -49.9 5.1'
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
  fp = water
  pressure_unit = Pa
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
    args = 'enclosing_element_normal_length enclosing_element_normal_thermal_cond'
    constant_names = h_s
    constant_expressions = 1E3 #This is the value being assigned to h_s.   Should be much bigger than thermal_conductivity / L ~ 1
    function = 'if(enclosing_element_normal_length = 0, 0, h_s * enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length / (h_s * enclosing_element_normal_length * enclosing_element_normal_length + enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length))'
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
    point = '0.1 -50 0'
    variable = frac_P
  []
  [withdraw_fluid]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = kg_out_uo
    bottom_p_or_t = 30.6e6
    character = 1
    line_length = 1
    point_file = production_single_fracture_amr.xyz
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
    point_file = production_single_fracture_amr.xyz
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
    porosity_ref = 1E-4
    P_ref = insitu_pp
    P_coeff = 3e-10
    porosity_min = 1E-5
  []
  [permeability]
    type = PorousFlowPermeabilityKozenyCarman
    k0 = 1E-15
    poroperm_function = kozeny_carman_phi0
    m = 0
    n = 3
    phi0 = 1E-4
  []
  [internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2700
    specific_heat_capacity = 0
  []
  [aq_thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '0.6E-4 0 0  0 0.6E-4 0  0 0 0.6E-4'
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
    value = '9.81*1000*(3000 - z)'
  []
  [insitu_T]
    type = ParsedFunction
    value = '363'
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
  []
  [kg_per_s]
    type = FunctionValuePostprocessor
    function = kg_rate
  []
  [J_out]
    type = PorousFlowPlotQuantity
    uo = J_out_uo
  []
  [TK_in]
    type = PointValue
    variable = frac_T
    point = '0.1 -50 0'
  []
  [TK_out]
    type = PointValue
    variable = frac_T
    point = '0.1 50 0'
  []
  [P_in]
    type = PointValue
    variable = frac_P
    point = '0.1 -50 0'
  []
  [P_out]
    type = PointValue
    variable = frac_P
    point = '0.1 50 0'
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
  dt = 0.1e7
  end_time = ${endTime}
  line_search = 'none'
  automatic_scaling = true
  l_max_its = 20
  l_tol = 8e-3
  nl_forced_its = 1
  nl_max_its = 20
  nl_rel_tol = 5e-04
  nl_abs_tol = 1e-09
[]

[Outputs]
  print_linear_residuals = false
  exodus = false
  csv = true
  # [fracCSV]
  #   type = CSV
  #   sync_times = '100 200 300 400 500 600 700 800 900
  #                 1000 2000 3000 4000 5000 6000 7000 8000 9000
  #                 1000e1 2000e1 3000e1 4000e1 5000e1 6000e1 7000e1 8000e1 9000e1
  #                 1000e2 2000e2 3000e2 4000e2 5000e2 6000e2 7000e2 8000e2 9000e2
  #                 1000e3 2000e3 3000e3 4000e3 5000e3 6000e3 7000e3 8000e3 9000e3
  #                 1000e4 2000e4 3000e4 4000e4 5000e4 6000e4 7000e4 8000e4 9000e4
  #                 1000e5 2000e5 3000e5 4000e5 5000e5 6000e5 7000e5 8000e5 9000e5'
  #   sync_only = true
  # []
[]
