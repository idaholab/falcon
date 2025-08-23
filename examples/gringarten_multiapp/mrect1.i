endTime = 3e8
frac_half_space = 20 #50
n_elem_x = 7 #10

[Mesh]
  # [matrixmesh]
  #   type = FileMeshGenerator # reading mesh from file
  #   file = 'gringartenbrickdomain100by100by10.e' #mesh created in cubit
  # []
  [left_side]
    type = GeneratedMeshGenerator
    dim = 3
    nx = ${n_elem_x}
    xmin = '${fparse -frac_half_space}'
    xmax = 0
    bias_x = 0.8
    ny = 50
    ymin = -50
    ymax = 50
    nz = 1
    zmin = -5
    zmax = 5
  []
  [right_side]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 7
    xmin = 0
    xmax = '${fparse frac_half_space}'
    bias_x = 1.2
    ny = 50
    ymin = -50
    ymax = 50
    nz = 1
    zmin = -5
    zmax = 5
  []
  [right_side_rename]
    type = RenameBoundaryGenerator
    input = right_side
    old_boundary = 'top bottom front back'
    new_boundary = 'rtop rbottom rfront rback'
  []
  [smg]
    type = StitchedMeshGenerator
    inputs = 'left_side right_side_rename'
    clear_stitched_boundary_ids = true
    stitch_boundaries_pairs = 'right left'
  []
[]

[GlobalParams]
  PorousFlowDictator = dictator
[]

[Variables]
  [matrix_P]
  []
  [matrix_T]
  []
[]

[ICs]
  [matrix_P]
    type = FunctionIC
    variable = matrix_P
    function = insitu_pp
  []
  [matrix_T]
    type = FunctionIC
    variable = matrix_T
    function = insitu_T
  []
[]

[BCs]
[]

[Functions]
  [insitu_pp]
    type = ParsedFunction
    expression = '9.81*1000*(3000 - z)'
  []
  [insitu_T]
    type = ParsedFunction
    expression = '363'
  []
[]

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = matrix_P
  temperature = matrix_T
  fp = water
  gravity = '0 0 -9.81'
  pressure_unit = Pa
[]

[FluidProperties]
  [water]
    type = SimpleFluidProperties
    thermal_expansion = 0
  []
[]

[Materials]
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1E-16 0 0   0 1E-16 0   0 0 1E-16'
  []
  [internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2875
    specific_heat_capacity = 825
  []
  [aq_thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.83 0 0  0 2.83 0  0 0 2.83'
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
  [fracDensity_AMR]
  []
[]

[AuxKernels]
  [normal_thermal_conductivity_auxk]
    type = ConstantAux
    variable = normal_thermal_conductivity
    value = 5
  []
  [element_normal_length_auxk]
    type = PorousFlowElementLength
    variable = element_normal_length
    direction = 'fracture_normal_x fracture_normal_y fracture_normal_z'
  []
[]

[MultiApps]
  [fracture_app]
    type = TransientMultiApp
    input_files = frect1.i
    execute_on = TIMESTEP_BEGIN
    sub_cycling = true
  []
[]

[Transfers]
  [normal_x_from_fracture]
    type = MultiAppNearestNodeTransfer
    from_multi_app = fracture_app
    source_variable = normal_dirn_x
    variable = fracture_normal_x
  []
  [normal_y_from_fracture]
    type = MultiAppNearestNodeTransfer
    from_multi_app = fracture_app
    source_variable = normal_dirn_y
    variable = fracture_normal_y
  []
  [normal_z_from_fracture]
    type = MultiAppNearestNodeTransfer
    from_multi_app = fracture_app
    source_variable = normal_dirn_z
    variable = fracture_normal_z
  []
  [element_normal_length_to_fracture]
    type = MultiAppNearestNodeTransfer
    to_multi_app = fracture_app
    source_variable = element_normal_length
    variable = enclosing_element_normal_length
  []
  [element_normal_thermal_cond_to_fracture]
    type = MultiAppNearestNodeTransfer
    to_multi_app = fracture_app
    source_variable = normal_thermal_conductivity
    variable = enclosing_element_normal_thermal_cond
  []
  [T_to_fracture]
    type = MultiAppGeometricInterpolationTransfer
    to_multi_app = fracture_app
    source_variable = matrix_T
    variable = transferred_matrix_T
  []
  [heat_from_fracture]
    type = MultiAppReporterTransfer
    from_multi_app = fracture_app
    from_reporters = 'heat_transfer_rate/joules_per_s heat_transfer_rate/x heat_transfer_rate/y heat_transfer_rate/z'
    to_reporters = 'heat_transfer_rate/transferred_joules_per_s heat_transfer_rate/x heat_transfer_rate/y heat_transfer_rate/z'
  []
[]
