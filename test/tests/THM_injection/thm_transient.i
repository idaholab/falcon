#### A fully-coupled T-H-M simulation of
#### cold water injection into a hot reservoir
#### through the center of a pre-existing fault disk
#### Part 2.
####   An injection process that lasts six days

#### Author of this script: Andy Wilkins of CSIRO

############################################################
[Mesh]
  [./file]
    type = FileMeshGenerator
    file = thm_transient_ic.e # use thm_steady_out.e for actual
  [../]
  [./source_position]
    type = ExtraNodesetGenerator
    input = file
    coord = '0 0 -2.5E3'
    tolerance = 50 # 20 for coarse and fine mesh | 50 for vcoarse mesh
    new_boundary = 7
  [../]
[]
############################################################
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  PorousFlowDictator = dictator
[]
############################################################
[Variables]
  [./P]
  [../]
  [./T]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]
############################################################
[ICs]
  [./P]
    type = FunctionIC
    function = initial_p
    variable = P
  [../]
  [./T]
    type = FunctionIC
    function = initial_t
    variable = T
  [../]
[]
############################################################
[Functions]
  [./initial_p]
    type = SolutionFunction
    from_variable = P
    solution = steady_solution
  [../]
  [./initial_t]
    type = SolutionFunction
    from_variable = T
    solution = steady_solution
  [../]
  [./initial_stress_xx]
    type = SolutionFunction
    from_variable = stress_xx
    solution = steady_solution
  [../]
  [./initial_stress_xy]
    type = SolutionFunction
    from_variable = stress_xy
    solution = steady_solution
  [../]
  [./initial_stress_yy]
    type = SolutionFunction
    from_variable = stress_yy
    solution = steady_solution
  [../]
  [./initial_stress_zz]
    type = SolutionFunction
    from_variable = stress_zz
    solution = steady_solution
  [../]
[]
############################################################
[Kernels]
  [./P_time_deriv]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = P
  [../]
  [./P_flux]
    type = PorousFlowAdvectiveFlux
    fluid_component = 0
    variable = P
    gravity = '0 0 -9.8'
  [../]
  [./energy_dot]
    type = PorousFlowEnergyTimeDerivative
    variable = T
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
  [./grad_stress_x]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./grad_stress_y]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
  [./grad_stress_z]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
  [../]
  [./gravity_z]
    type = Gravity
    variable = disp_z
    value = -9.8
  [../]
  [./poro_x]
    type = PorousFlowEffectiveStressCoupling
    biot_coefficient = 1.0
    variable = disp_x
    component = 0
  [../]
  [./poro_y]
    type = PorousFlowEffectiveStressCoupling
    biot_coefficient = 1.0
    variable = disp_y
    component = 1
  [../]
  [./poro_z]
    type = PorousFlowEffectiveStressCoupling
    biot_coefficient = 1.0
    variable = disp_z
    component = 2
  [../]
  [./poro_vol_exp]
    type = PorousFlowMassVolumetricExpansion
    variable = P
    fluid_component = 0
  [../]
  [./heat_vol_exp]
    type = PorousFlowHeatVolumetricExpansion
    variable = T
  [../]
[]
############################################################
[BCs]
  # 1 = zmax
  # 2 = zmin
  # 3 = xmin
  # 4 = xmax
  # 5 = ymin
  # 6 = ymax
  [./P_top]
    type = FunctionDirichletBC
    variable = P
    boundary = '1'
    function = '101325-z*9800'
  [../]
  [./T_sides_top_bot]
    type = FunctionDirichletBC
    variable = T
    boundary = '1 2 3 4 5 6'
    function = initial_t
  [../]
  [./roller_x]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = '3 4'
  [../]
  [./roller_y]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = '5 6'
  [../]
  [./bottom_z]
    type = DirichletBC
    variable = disp_z
    value = 0
    boundary = '2'
  [../]
  [./top_z]
    type = Pressure
    variable = disp_z
    component = 2
    function = '-2386.0*9.8*z'
    use_displaced_mesh = false
    boundary = '1'
  [../]
  [./P_source]
    type = FunctionDirichletBC
    variable = P
    boundary = 7
    function = 'if(t<=6*3600*24,101325-z*9800+30E6*t/6.0/3600.0/24.0,101325-z*9800)' # linearly increase to 30MPa over 6 days
  [../]
  [./T_source]
    type = FunctionDirichletBC
    variable = T
    boundary = 7
    function = 293
  [../]
[]
############################################################
[AuxVariables]
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_xz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./f_shear]
    order = CONSTANT
    family = MONOMIAL
    block = 1
  [../]
[]
############################################################
[AuxKernels]
  [./stress_xx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xx
    index_i = 0
    index_j = 0
  [../]
  [./stress_xy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xy
    index_i = 0
    index_j = 1
  [../]
  [./stress_xz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xz
    index_i = 0
    index_j = 2
  [../]
  [./stress_yx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yx
    index_i = 1
    index_j = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  [../]
  [./stress_yz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yz
    index_i = 1
    index_j = 2
  [../]
  [./stress_zx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zx
    index_i = 2
    index_j = 0
  [../]
  [./stress_zy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zy
    index_i = 2
    index_j = 1
  [../]
  [./stress_zz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zz
    index_i = 2
    index_j = 2
  [../]
  [./f_shear]
    type = MaterialStdVectorAux
    property = plastic_yield_function
    index = 0
    variable = f_shear
  [../]
[]
############################################################
[UserObjects]
  [./steady_solution]
    type = SolutionUserObject
    timestep = 2
    mesh = thm_transient_ic.e
  [../]
  [./dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'P T disp_x disp_y disp_z'
    number_fluid_phases = 1
    number_fluid_components = 1
  [../]
  [./coh]
    type = TensorMechanicsHardeningConstant
    value = 1E6
  [../]
  [./tanphi]
    type = TensorMechanicsHardeningConstant
    value = 0.85
  [../]
  [./tanpsi]
    type = TensorMechanicsHardeningConstant
    value = 0.01
  [../]
  [./t_strength]
    type = TensorMechanicsHardeningConstant
    value = 1E16
  [../]
  [./c_strength]
    type = TensorMechanicsHardeningConstant
    value = 1E16
  [../]
  [./ts]
    # large so that there is no plastic deformation
    type = TensorMechanicsHardeningConstant
    value = 1E16
  [../]
  [./mc]
    # irrelevant for this simulation
    type = TensorMechanicsPlasticTensile
    tensile_strength = ts
    yield_function_tolerance = 1E-6
    tensile_tip_smoother = 1.0
    internal_constraint_tolerance = 1E-5
  [../]
  [./pc]
    type = PorousFlowCapillaryPressureVG
    alpha = 1E-8
    m = 0.5
  [../]
[]
############################################################
[./FluidProperties]
  [./fluid]
    type = SimpleFluidProperties
    density0 = 999.526
    bulk_modulus = 2E9
    viscosity = 0.001
    cv = 4180
    thermal_expansion = 0
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
  [./rock_heat]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 920.0
    density = 2540.0
  [../]
  [./fluid_props]
    type = PorousFlowSingleComponentFluid
    fp = fluid
    phase = 0
    at_nodes = true
  [../]
  [./fluid_props_qp]
    type = PorousFlowSingleComponentFluid
    phase = 0
    fp = fluid
  [../]
  [./thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '1.0 0 0  0 1.0 0  0 0 1.0'
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
  [./permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1.0E-12 0 0  0 1.0E-12 0  0 0 1.0E-12'
  [../]
  [./relperm]
    type = PorousFlowRelativePermeabilityCorey
    at_nodes = true
    n = 1
    phase = 0
  [../]
  [./porosity]
    type = PorousFlowPorosityConst
    at_nodes = true
    porosity = 0.1
  [../]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '8.65E9 5.77E9' # young = 15GPa, poisson = 0.3
    fill_method = symmetric_isotropic
  [../]
  [./thermal_expansion_strain]
    type = ComputeThermalExpansionEigenstrain
    stress_free_temperature = 293
    thermal_expansion_coeff = 1.0E-5
    temperature = T
    eigenstrain_name = eigenstrain
  [../]
  [./strain]
    type = ComputeIncrementalSmallStrain
    eigenstrain_names = 'ini_stress'
  [../]
  [./strain_from_initial_stress]
    type = ComputeEigenstrainFromInitialStress
    initial_stress = 'initial_stress_xx initial_stress_xy 0  initial_stress_xy initial_stress_yy 0  0 0 initial_stress_zz'
    eigenstrain_name = ini_stress
  [../]
  [./stress_rock]
    type = ComputeMultiPlasticityStress
    block = 2
    # the rest of this stuff is irrelevant for this test
    ep_plastic_tolerance = 1E-5
    plastic_models = mc
    debug_fspb = crash
  [../]
  [./admissible]
    type = ComputeMultipleInelasticStress
    inelastic_models = stress_joint
    perform_finite_strain_rotations = false
    block = 1
  [../]
  [./stress_joint]
    type = CappedWeakInclinedPlaneStressUpdate
    block = 1
    normal_vector = '0 1 0'
    cohesion = coh
    tan_friction_angle = tanphi
    tan_dilation_angle = tanpsi
    tensile_strength = t_strength
    compressive_strength = c_strength
    tip_smoother = 1.0E5
    smoothing_tol = 1.0
    yield_function_tol = 1.0
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = density
    prop_values = 2386.0 # = (1-0.9)*2540 + 0.1*999.526
  [../]
  [./eff_fluid_pressure]
    type = PorousFlowEffectiveFluidPressure
    at_nodes = true
  [../]
  [./eff_fluid_pressure_qp]
    type = PorousFlowEffectiveFluidPressure
  [../]
  [./vol_strain]
    type = PorousFlowVolumetricStrain
  [../]
[]
############################################################
[Postprocessors]
  [./fmax]
    type = ElementExtremeValue
    block = 1
    variable = f_shear
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
[]
############################################################
[Executioner]
  type = Transient
  num_steps = 1 # 6 for full run | 1 for test
  dt = 86400.0 # 1 day

  solve_type = 'NEWTON' # default = PJFNK | NEWTON

  l_max_its  = 20
  l_tol      = 1e-4
  nl_max_its = 500
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-2
[]
############################################################
[Outputs]
  file_base      = thm_transient_out
  exodus         = true
  [./console]
    type = Console
    output_linear = true
    output_nonlinear = true
  [../]
[]
############################################################
