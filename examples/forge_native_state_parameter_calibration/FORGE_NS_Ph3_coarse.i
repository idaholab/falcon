#Revisions made to the original FORGE native state model
# NS mesh replaced with mesh generator to create a coarser mesh
# Transient simulation to reach equilibration with sources.
# Dirac kernel source for temperature and mass injection at the bottom of well 78
# 3 levels of mesh refinement around sources to increase sensitivity
# vectorpostprocessor Output along wells includes displacment and does not include stress

# original FORGE native state model can downloaded from the GDR website:
# https://gdr.openei.org/submissions/1397

#parameters being inverted for
K_Cond = 2.8
K_Perm = 1e-14

[Mesh]
  [generate]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 10
    xmin = -40
    xmax = 4040
    ny = 10
    ymin = -40
    ymax = 4040
    nz = 10
    zmin = -2360
    zmax = 1804
  []
  #ALL OF THESE SIDESET NAMES ARE MIXED UP WITH THE DEFAULT SIDESET NAMES
  [bottom_40m_granitoid_40m]
    type = RenameBlockGenerator
    input = generate
    old_block = '0'
    new_block = 'bottom_40m_granitoid_40m'
  []
  [granitoid_40m_surface_40m]
    type = ParsedSubdomainMeshGenerator
    input = bottom_40m_granitoid_40m
    combinatorial_geometry = 'z > 1000'
    block_id = 1
    block_name = granitoid_40m_surface_40m
  []
  [bottom_40m_granitoid_40m_right]
    type = ParsedGenerateSideset
    input = granitoid_40m_surface_40m
    included_boundaries = bottom
    combinatorial_geometry = 'z < 1000'
    new_sideset_name = 'bottom_40m_granitoid_40m_right'
  []
  [granitoid_40m_surface_40m_right]
    type = ParsedGenerateSideset
    input = bottom_40m_granitoid_40m_right
    included_boundaries = bottom
    combinatorial_geometry = 'z > 1000'
    new_sideset_name = 'granitoid_40m_surface_40m_right'
  []
  [bottom_40m_granitoid_40m_left]
    type = ParsedGenerateSideset
    input = granitoid_40m_surface_40m_right
    included_boundaries = top
    combinatorial_geometry = 'z < 1000'
    new_sideset_name = 'bottom_40m_granitoid_40m_left'
  []
  [granitoid_40m_surface_40m_left]
    type = ParsedGenerateSideset
    input = bottom_40m_granitoid_40m_left
    included_boundaries = top
    combinatorial_geometry = 'z > 1000'
    new_sideset_name = 'granitoid_40m_surface_40m_left'
  []
  [bottom_40m_granitoid_40m_front]
    type = ParsedGenerateSideset
    input = granitoid_40m_surface_40m_left
    included_boundaries = left
    combinatorial_geometry = 'z < 1000'
    new_sideset_name = 'bottom_40m_granitoid_40m_front'
  []
  [granitoid_40m_surface_40m_front]
    type = ParsedGenerateSideset
    input = bottom_40m_granitoid_40m_front
    included_boundaries = left
    combinatorial_geometry = 'z > 1000'
    new_sideset_name = 'granitoid_40m_surface_40m_front'
  []
  [bottom_40m_granitoid_40m_back]
    type = ParsedGenerateSideset
    input = granitoid_40m_surface_40m_front
    included_boundaries = right
    combinatorial_geometry = 'z < 1000'
    new_sideset_name = 'bottom_40m_granitoid_40m_back'
  []
  [granitoid_40m_surface_40m_back]
    type = ParsedGenerateSideset
    input = bottom_40m_granitoid_40m_back
    included_boundaries = right
    combinatorial_geometry = 'z > 1000'
    new_sideset_name = 'granitoid_40m_surface_40m_back'
  []

  [rename]
    type = RenameBoundaryGenerator
    input = granitoid_40m_surface_40m_back
    old_boundary = 'front back'
    new_boundary = 'surface_40m bottom_40m'
  []

  [add_internal_sideset]
    type = SideSetsBetweenSubdomainsGenerator
    input = rename
    primary_block = 0
    paired_block = 1
    new_boundary = granitoid_40m
  []

  use_displaced_mesh = false
[]

[Postprocessors]
  [mass_flux_in]
    type = FunctionValuePostprocessor
    function = mass_flux_in_fn
    execute_on = 'initial timestep_end'
  []
  [T_in]
    type = FunctionValuePostprocessor
    function = T_in_fn
    execute_on = 'initial timestep_end'
  []
[]

[Functions]
  [mass_flux_in_fn]
    type = PiecewiseLinear
    xy_data = '
      0   0.0
      50  0.1
      100  0.1'
  []
  [T_in_fn]
    type = PiecewiseLinear
    xy_data = '
      0   323.15
      50   323.15
      100  323.15'
  []
[]

[DiracKernels]
  [source]
    #placed at the bottom of well 78_32
    type = PorousFlowPointSourceFromPostprocessor
    variable = pressure
    mass_flux = mass_flux_in
    point = '2327.3 1795.9 679.59'
  []
  [source_h]
    #placed at the bottom of well 78_32
    type = PorousFlowPointEnthalpySourceFromPostprocessor
    variable = temperature
    mass_flux = mass_flux_in
    point = '2327.3 1795.9 679.59'
    T_in = T_in
    pressure = pressure
    fp = true_water
  []
[]

[Adaptivity]
  #refine around DiracKernels sources
  # should refine more than one level but this should run fast
  initial_steps = 2
  max_h_level = 1
  marker = box
  [Markers]
    [box]
      type = BoxMarker
      bottom_left = '2027 1495 379'
      top_right = '2527 2095 879'
      inside = refine
      outside = do_nothing
    []
  []
[]

[Executioner]
  line_search = None
  type = Transient
  solve_type = NEWTON
  steady_state_detection = true
  steady_state_tolerance = 1e-8
  dt = 10
  end_time = 100
  start_time = 0
  l_max_its = 1000
  nl_max_its = 100
  l_tol = 1e-6
  nl_abs_tol = 1e-7
  nl_rel_tol = 1e-6
  reuse_preconditioner = true
[]

#############################################################
[GlobalParams]
  PorousFlowDictator = dictator
  displacements = 'disp_i disp_j disp_k'
[]
############################################################
[Variables]
  [pressure]
  []

  [temperature]
    scaling = 1E-8
  []

  [disp_i]
    scaling = 1E-10
  []

  [disp_j]
    scaling = 1E-10
  []

  [disp_k]
    scaling = 1E-10
  []
[]

############################################################
[ICs]
  [temperature]
    type = FunctionIC
    variable = temperature
    function = '616 - ((2360+z)*7.615e-2)'
  []

  [pressure]
    type = FunctionIC
    variable = pressure
    function = '3.5317e7 - ((2360+z)*8455)'
  []
[]

############################################################
[Kernels]
  [Darcy_flow]
    type = PorousFlowAdvectiveFlux
    fluid_component = 0
    variable = pressure
    gravity = '0 0 -9.8'
  []

  [heat_conduction]
    type = PorousFlowHeatConduction
    variable = temperature
  []

  [grad_stress_i]
    type = StressDivergenceTensors
    variable = disp_i
    temperature = temperature
    component = 0
  []

  [grad_stress_j]
    type = StressDivergenceTensors
    temperature = temperature
    variable = disp_j
    component = 1
  []

  [grad_stress_k]
    type = StressDivergenceTensors
    temperature = temperature
    variable = disp_k
    component = 2
  []

  [poro_i]
    type = PoroMechanicsCoupling
    variable = disp_i
    porepressure = pressure
    component = 0
  []

  [poro_j]
    type = PoroMechanicsCoupling
    variable = disp_j
    porepressure = pressure
    component = 1
  []

  [poro_k]
    type = PoroMechanicsCoupling
    variable = disp_k
    porepressure = pressure
    component = 2
  []

  [weight]
    type = Gravity
    variable = disp_k
    value = -9.8
  []
[]

###########################################################
[AuxVariables]
  [stress_ii]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_ij]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_ik]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_ji]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_jj]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_jk]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_ki]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_kj]
    order = CONSTANT
    family = MONOMIAL
  []

  [stress_kk]
    order = CONSTANT
    family = MONOMIAL
  []

  [vonmises]
    order = CONSTANT
    family = MONOMIAL
  []

  [hydrostatic]
    order = CONSTANT
    family = MONOMIAL
  []

  [density]
    order = CONSTANT
    family = MONOMIAL
  []

  [viscosity]
    order = CONSTANT
    family = MONOMIAL
  []
[]

#********************************************************
[Functions]
  [T_on_face_Z_Minus]
    type = PiecewiseBilinear
    data_file = FORGE_bottom_2021.11.10_plus_50.csv
    xaxis = 1
    yaxis = 0
  []
[]

##############################################################
[AuxKernels]
  [stress_ii]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_ii
    index_i = 0
    index_j = 0
  []

  [stress_ij]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_ij
    index_i = 0
    index_j = 1
  []

  [stress_ik]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_ik
    index_i = 0
    index_j = 2
  []

  [stress_ji]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_ji
    index_i = 1
    index_j = 0
  []

  [stress_jj]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_jj
    index_i = 1
    index_j = 1
  []

  [stress_jk]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_jk
    index_i = 1
    index_j = 2
  []

  [stress_ki]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_ki
    index_i = 2
    index_j = 0
  []

  [stress_kj]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_kj
    index_i = 2
    index_j = 1
  []

  [stress_kk]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_kk
    index_i = 2
    index_j = 2
  []

  [density]
    type = MaterialRealAux
    variable = density
    property = PorousFlow_fluid_phase_density_qp0
    execute_on = TIMESTEP_END
  []

  [viscosity]
    type = MaterialRealAux
    variable = viscosity
    property = PorousFlow_viscosity_qp0
    execute_on = TIMESTEP_END
  []

  [vonmises]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    variable = vonmises
    scalar_type = VonMisesStress
  []

  [hydrostatic]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    variable = hydrostatic
    scalar_type = Hydrostatic
  []
[]
############################################################
[UserObjects]
  [dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'pressure temperature disp_i disp_j disp_k'
    number_fluid_phases = 1
    number_fluid_components = 1
  []

  [pc]
    type = PorousFlowCapillaryPressureVG
    alpha = 1
    m = 0.5
  []

[]
############################################################
[BCs]
  # B.C. for solid field
  # Prescribed displacements at three boundary surfaces
  [roller_sigma_h_min_x_minus]
    type = DirichletBC
    preset = true
    variable = disp_i
    value = 0
    boundary = 'bottom_40m_granitoid_40m_front granitoid_40m_surface_40m_front'
  []

  [roller_sigma_h_min_x_plus]
    type = DirichletBC
    preset = true
    variable = disp_i
    value = 0
    boundary = 'bottom_40m_granitoid_40m_back granitoid_40m_surface_40m_back'
  []

  [roller_sigma_h_max_minus]
    type = DirichletBC
    preset = true
    variable = disp_j
    value = 0
    boundary = 'bottom_40m_granitoid_40m_right granitoid_40m_surface_40m_right'
  []

  [bottom_z]
    type = DirichletBC
    preset = true
    variable = disp_k
    value = 0
    boundary = 'bottom_40m'
  []

  # Prescribed traction
  [S_h_max_normal_righ]
    type = FunctionNeumannBC
    variable = disp_j
    boundary = 'bottom_40m_granitoid_40m_left granitoid_40m_surface_40m_left'
    function = '-19889*(1785-z)'
  []

  [S_h_max_shear_right]
    type = FunctionNeumannBC
    variable = disp_k
    boundary = 'bottom_40m_granitoid_40m_left granitoid_40m_surface_40m_left'
    function = '929*(1785-z)'
  []

  [S_h_max_shear_left]
    type = FunctionNeumannBC
    variable = disp_k
    boundary = 'bottom_40m_granitoid_40m_right granitoid_40m_surface_40m_right'
    function = '-929*(1785-z)'
  []

  [Side_Z_minus_traction_Z]
    type = NeumannBC
    variable = disp_j
    boundary = 'bottom_40m'
    value = -3.85e+6
  []

  [Side_Z_Plus_traction_Z]
    type = NeumannBC
    variable = disp_k
    boundary = 'surface_40m'
    value = -101325
  []

  #B.C. for Darcy's flow field
  # Apply zero pore pressure on top surface
  # and no flow side surfaces
  [pore_pressure_top]
    type = DirichletBC
    variable = pressure
    boundary = 'surface_40m'
    value = 101325 #atmospheric pressure in Pa-assumes water level at ground surface
  []

  [pore_pressure_bottom]
    type = DirichletBC
    variable = pressure
    boundary = 'bottom_40m'
    value = 34000000
  []

  #B.C. Temperature field
  # Apply prescibed temperature on top and bottom surfaces
  # and no heat flux on the side surfaces

  [Side_Z_Minus_T]
    type = FunctionDirichletBC
    #type = DirichletBC
    variable = temperature
    boundary = 'bottom_40m'
    function = T_on_face_Z_Minus
  []

  [Side_Z_Plus_T]
    type = DirichletBC
    variable = temperature
    boundary = 'surface_40m'
    value = 299
  []
[]

############################################################

[FluidProperties]
  [true_water]
    type = Water97FluidProperties
  []
  [tabulated_water]
    type = TabulatedBicubicFluidProperties
    fp = true_water
    fluid_property_file = tabulated_fluid_properties_v2.csv
  []
[]

############################################################
[Materials]
  #fluid properties and flow
  [permeability_sediment]
    type = PorousFlowPermeabilityConst
    permeability = '${K_Perm} 0 0  0 ${K_Perm} 0  0 0 ${K_Perm}'
    block = granitoid_40m_surface_40m
  []

  [permeability_granite]
    type = PorousFlowPermeabilityConst
    permeability = '1e-18 0 0  0 1e-18 0  0 0 1e-18'
    block = bottom_40m_granitoid_40m
  []

  [fluid_props]
    type = PorousFlowSingleComponentFluid
    fp = tabulated_water
    phase = 0
    at_nodes = true
  []

  [fluid_props_qp]
    type = PorousFlowSingleComponentFluid
    phase = 0
    fp = tabulated_water
  []

  [porosity_sediment]
    type = PorousFlowPorosityConst
    porosity = 0.12
    block = granitoid_40m_surface_40m
  []

  [porosity_granite]
    type = PorousFlowPorosityConst
    porosity = 0.001
    block = bottom_40m_granitoid_40m
  []

  [ppss]
    type = PorousFlow1PhaseP
    at_nodes = true
    porepressure = pressure
    capillary_pressure = pc
  []

  [ppss_qp]
    type = PorousFlow1PhaseP
    porepressure = pressure
    capillary_pressure = pc
  []

  [massfrac]
    type = PorousFlowMassFraction
    at_nodes = true
  []

  [eff_fluid_pressure]
    type = PorousFlowEffectiveFluidPressure
    at_nodes = true
  []

  [eff_fluid_pressure_qp]
    type = PorousFlowEffectiveFluidPressure
  []

  [relperm]
    type = PorousFlowRelativePermeabilityCorey
    at_nodes = true
    n = 1
    phase = 0
  []

  #energy transport
  [temperature]
    type = PorousFlowTemperature
    temperature = temperature
  []

  [temperature_nodal]
    type = PorousFlowTemperature
    at_nodes = true
    temperature = temperature
  []

  [rock_heat_sediments]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 830.0
    density = 2500
    block = granitoid_40m_surface_40m
    #density from Rick allis analysis, specific heat per tabulated values
  []

  [rock_heat_granitoid]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 790.0
    density = 2750
    block = bottom_40m_granitoid_40m
    #density from Rick allis analysis, specific heat per tabulated values
  []

  [thermal_conductivity_sediments]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '${K_Cond} 0 0  0 ${K_Cond} 0  0 0 ${K_Cond}'
    block = granitoid_40m_surface_40m
  []

  [thermal_conductivity_granitoid]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '3.05 0 0  0 3.05 0  0 0 3.05'
    block = bottom_40m_granitoid_40m
  []

  #mechanics
  [density_sediment]
    type = GenericConstantMaterial
    prop_names = density
    block = granitoid_40m_surface_40m
    prop_values = 2500.0
  []

  [density_granitoid]
    type = GenericConstantMaterial
    prop_names = density
    block = bottom_40m_granitoid_40m
    prop_values = 2750.0
  []

  [biot]
    type = GenericConstantMaterial
    prop_names = biot_coefficient
    prop_values = 0.47
  []

  [elasticity_sediment]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 3.0e+10
    poissons_ratio = 0.30
    block = granitoid_40m_surface_40m
  []

  [elasticity_granitoid]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 6.2e+10
    poissons_ratio = 0.3
    block = bottom_40m_granitoid_40m
  []

  [thermal_expansion_strain]
    type = ComputeThermalExpansionEigenstrain
    stress_free_temperature = 293
    thermal_expansion_coeff = 6.0E-6
    temperature = temperature
    eigenstrain_name = eigenstrain
  []

  [strain]
    type = ComputeSmallStrain
  []

  [stress]
    type = ComputeLinearElasticStress
  []
[]

############################################################
[Preconditioning]
  active = 'preferred'

  [ilu_may_use_less_mem]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type'
    petsc_options_value = 'gmres asm ilu NONZERO'
  []

  [superlu]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'gmres lu superlu_dist'
  []

  [preferred]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
    petsc_options_value = ' lu       mumps'
  []
[]

[VectorPostprocessors]
  [16A]
    type = PointValueSampler
    variable = 'temperature pressure disp_i disp_j disp_k'
    points = '1102.5 1818.6    1625.19
  1102.3 1818.7    1600.19
  1102.2    1818.8    1575.19
  1102.1    1818.8    1550.19
  1102    1818.8    1525.19
  1102.1    1818.8    1500.19
  1102.2    1818.6    1475.19
  1102.3    1818.4    1450.19
  1102.3    1818.2    1425.19
  1102.4    1817.8    1400.19
  1102.5    1817.4    1375.2
  1102.6    1816.7    1350.21
  1102.8    1816    1325.22
  1103.1    1815.2    1300.23
  1103.4    1814.4    1275.25
  1103.8    1813.8    1250.26
  1104    1813.3    1225.26
  1104.3    1813    1200.27
  1104.6    1812.9    1175.27
  1104.9    1812.8    1150.27
  1105.2    1812.8    1125.27
  1105.6    1813    1100.28
  1105.8    1813.2    1075.28
  1105.9    1813.6    1050.28
  1105.6    1814    1025.29
  1105.2    1814    1000.29
  1104.9    1813.9    975.29
  1104.8    1813.6    950.29
  1104.9    1813.5    925.3
  1105.2    1813.5    900.3
  1105.3    1813.7    875.3
  1105.2    1814.1    850.3
  1104.9    1814.4    825.31
  1104.5    1814.3    800.31
  1104.3    1814.2    775.31
  1104.4    1814    750.31
  1104.8    1813.9    725.32
  1105.3    1814.2    700.32
  1105.4    1814.8    675.33
  1105.3    1815.3    650.34
  1104.9    1815.5    625.34
  1104.5    1815.5    600.34
  1104.4    1815.3    575.34
  1104.5    1815.1    550.34
  1104.8    1815.3    525.35
  1105.1    1815.7    500.35
  1105.3    1816.3    475.36
  1105.2    1817    450.37
  1104.9    1817.5    425.38
  1104.5    1817.7    400.38
  1104.1    1817.6    375.39
  1104    1817.3    350.39
  1104.1    1816.9    325.39
  1104.4    1816.9    300.39
  1104.6    1817.3    275.4
  1104.4    1817.8    250.4
  1103.9    1818    225.41
  1103.4    1817.9    200.42
  1103.2    1817.5    175.42
  1103.4    1817    150.43
  1104    1816.7    125.43
  1104.6    1816.7    100.44
  1105.2    1816.9    75.45
  1105.7    1817.3    50.46
  1106.1    1817.8    25.47
  1106.2    1818.3    0.47
  1106.2    1818.5    -24.53
  1106.4    1818.5    -49.53
  1107    1818.3    -74.52
  1107.8    1818.3    -99.51
  1108.7    1818.2    -124.49
  1110.1    1818.4    -149.45
  1113    1819.3    -174.25
  1117.9    1820.8    -198.71
  1124.6    1822.2    -222.77
  1132.2    1822.9    -246.57
  1139.9    1823.2    -270.33
  1148.1    1823    -293.96
  1157.4    1822.4    -317.16
  1168.3    1821.5    -339.62
  1181.1    1820.8    -361.05
  1195.8    1821.2    -381.25
  1211.8    1823.4    -400.34
  1229.3    1826.3    -417.94
  1248.3    1829.5    -433.88
  1268.6    1832.5    -448.14
  1289.9    1835.4    -460.88
  1311.9    1838.4    -472.44
  1334.3    1841.8    -482.88
  1356.9    1845    -493.1
  1379.5    1847.8    -503.42
  1402.1    1849.9    -513.86
  1424.6    1851.2    -524.79
  1446.3    1851.7    -537.2
  1467.3    1851.8    -550.66
  1488.2    1851.8    -564.37
  1509.5    1851.9    -577.52
  1531.4    1852.1    -589.56
  1553.9    1852.2    -600.42
  1576.6    1852.2    -610.87
  1599.5    1852.3    -621.07
  1622.3    1852.9    -631.11
  1645.2    1853.7    -641.19
  1668.1    1854.5    -651.3
  1691    1856.3    -661.09
  1714    1859.3    -670.49
  1737    1862.9    -679.51
  1759.9    1866.9    -688.68
  1782.6    1870.9    -698.32
  1805.2    1875    -708.33
  1827.8    1878.8    -718.33
  1850.4    1882    -728.33
  1873.1    1884.6    -738.49
  1895.7    1886.8    -749.04
  1918.1    1888.5    -760.07
  1940.3    1890    -771.38
  1962.5    1891.6    -782.84
  1984.6    1893.1    -794.3
  2007    1894.5    -805.42
  2029.6    1896.2    -815.88
  2052.5    1898.2    -825.72
  2075.4    1900.1    -835.6
  2098.2    1901.5    -845.77
  2120.9    1902.5    -856.23
  2143.4    1903.2    -867
  2165.9    1904.2    -877.91
  2188.4    1905.7    -888.63
  2211    1907.6    -899.21
  2233.9    1910.2    -908.97
  2257    1913.6    -917.94
  2280    1917.6    -926.72
  2303    1921.8    -935.78
  2325.9    1926.2    -944.74
  2330.4    1927.1    -946.48'
    sort_by = z
  []

  [58_32]
    type = PointValueSampler
    variable = 'temperature pressure disp_i disp_j disp_k'
    points = '2002    1723.2    1659.78
  2002    1723.2    1634.78
  2002    1723.3    1609.78
  2002    1723.4    1584.78
  2002    1723.5    1559.78
  2002    1723.5    1534.78
  2002    1723.4    1509.78
  2001.9    1723.3    1484.78
  2001.9    1723.3    1459.79
  2001.9    1723.4    1434.79
  2001.8    1723.5    1409.79
  2001.7    1723.7    1384.79
  2001.6    1724    1359.79
  2001.4    1724.3    1334.79
  2001.1    1724.7    1309.8
  2000.9    1725    1284.8
  2000.6    1725.4    1259.8
  2000.4    1725.7    1234.81
  2000.1    1726.1    1209.81
  1999.8    1726.5    1184.82
  1999.5    1726.9    1159.82
  1999.1    1727.3    1134.83
  1998.8    1727.7    1109.83
  1998.5    1728.3    1084.84
  1998.4    1729    1059.85
  1998.4    1729.7    1034.86
  1998.5    1730.5    1009.87
  1998.8    1731    984.88
  1999.2    1731.4    959.89
  1999.6    1731.7    934.89
  1999.9    1731.9    909.89
  2000.1    1732.2    884.9
  2000.2    1732.4    859.9
  2000.2    1732.7    834.9
  2000    1733    809.9
  1999.8    1733.3    784.91
  1999.4    1733.5    759.91
  1999    1733.7    734.91
  1998.7    1733.8    709.92
  1998.8    1733.8    684.92
  1999.1    1733.8    659.92
  1999.5    1733.6    634.92
  1999.8    1733.3    609.93
  1999.8    1732.8    584.93
  1999.7    1732.2    559.94
  1999.3    1731.5    534.95
  1998.6    1730.7    509.98
  1997.6    1729.8    485.01
  1996.2    1728.8    460.07
  1994.6    1727.7    435.14
  1992.8    1726.4    410.24
  1990.7    1725    385.37
  1988.5    1723.4    360.52
  1986.2    1721.8    335.68
  1983.8    1720.1    310.85
  1981.4    1718.5    286.02
  1978.4    1717.6    261.22
  1975.5    1716.7    236.4
  1972.5    1715.8    211.6
  1969.9    1715.2    186.75
  1968    1714.6    161.82
  1967.1    1714    136.85
  1966.4    1713.8    111.86
  1965.8    1713.8    86.87
  1965.4    1713.4    61.87
  1965.1    1713    36.88
  1965.2    1712.8    11.88
  1965.3    1712.6    -13.12
  1965.5    1712.4    -38.12
  1965.7    1712.3    -63.12
  1965.5    1712.4    -88.12
  1965    1712.7    -113.11
  1964.4    1713.1    -138.1
  1964    1713.7    -163.09
  1963.5    1714.4    -188.07
  1963.1    1715.2    -213.06
  1962.5    1716    -238.04
  1962    1716.9    -263.02
  1961.3    1717.7    -287.99
  1960.4    1718.2    -312.97
  1959.3    1718.2    -337.95
  1957.8    1717.9    -362.9
  1956.2    1717.5    -387.84
  1954.8    1717.3    -412.8
  1954.1    1717    -437.79
  1953.7    1716.9    -462.79
  1953.2    1716.8    -487.78
  1952.6    1716.7    -512.78
  1952    1716.5    -537.77
  1951.2    1716.3    -562.75
  1950.5    1716    -587.74
  1949.8    1715.9    -609.63'
    sort_by = z
  []

  [78_32]
    type = PointValueSampler
    variable = 'temperature pressure disp_i disp_j disp_k'
    points = '2327.3    1795.9    1679.29
  2327.3    1795.9    1654.29
  2327.3    1795.9    1629.29
  2327.3    1795.9    1604.29
  2327.3    1795.9    1579.29
  2327.3    1795.9    1554.29
  2327.3    1795.9    1529.29
  2327.3    1795.9    1504.29
  2327.3    1795.9    1479.29
  2327.3    1795.9    1454.29
  2327.3    1795.9    1429.29
  2327.3    1795.9    1404.29
  2327.3    1795.9    1379.29
  2327.3    1795.9    1354.29
  2327.3    1795.9    1329.29
  2327.3    1795.9    1304.29
  2327.3    1795.9    1279.29
  2327.3    1795.9    1254.29
  2327.3    1795.9    1229.29
  2327.3    1795.9    1204.29
  2327.3    1795.9    1179.29
  2327.3    1795.9    1154.29
  2327.3    1795.9    1129.29
  2327.3    1795.9    1104.29
  2327.3    1795.9    1079.29
  2327.3    1795.9    1054.29
  2327.3    1795.9    1029.29
  2327.3    1795.9    1004.29
  2327.3    1795.9    979.29
  2327.3    1795.9    954.29
  2327.3    1795.9    929.29
  2327.3    1795.9    904.29
  2327.3    1795.9    879.29
  2327.3    1795.9    854.29
  2327.3    1795.9    829.29
  2327.3    1795.9    804.29
  2327.3    1795.9    779.29
  2327.3    1795.9    754.29
  2327.3    1795.9    729.29
  2327.3    1795.9    704.29
  2327.3    1795.9    679.59'
    sort_by = z
  []

  [78B_32]
    type = PointValueSampler
    variable = 'temperature pressure disp_i disp_j disp_k'
    points = '2410    1822.8    1687.37
  2410    1822.8    1662.37
  2410    1822.8    1637.37
  2410    1822.8    1612.37
  2410    1822.8    1587.37
  2410    1822.8    1562.37
  2410    1822.8    1537.37
  2410    1822.8    1512.37
  2410    1822.8    1487.37
  2410    1822.8    1462.37
  2410    1822.8    1437.37
  2410    1822.8    1412.37
  2410    1822.8    1387.37
  2410    1822.8    1362.37
  2410    1822.8    1337.37
  2410    1822.8    1312.37
  2410    1822.8    1287.37
  2410    1822.8    1262.37
  2410    1822.8    1237.37
  2410    1822.8    1212.37
  2410    1822.8    1187.37
  2410    1822.8    1162.37
  2410    1822.8    1137.37
  2410    1822.8    1112.37
  2410    1822.8    1087.37
  2410    1822.8    1062.37
  2410    1822.8    1037.37
  2410    1822.8    1012.37
  2410    1822.8    987.37
  2410    1822.8    962.37
  2410    1822.8    937.37
  2410    1822.8    912.37
  2410    1822.8    887.37
  2410    1822.8    862.37
  2410    1822.8    837.37
  2410    1822.8    812.37
  2410    1822.8    787.37
  2410    1822.8    762.37
  2410    1822.8    737.37
  2410    1822.8    712.37
  2410    1822.8    687.37
  2410    1822.8    662.37
  2410    1822.8    637.37
  2410    1822.8    612.37
  2410    1822.8    587.37
  2410    1822.8    562.37
  2410    1822.8    537.37
  2410    1822.8    512.37
  2410    1822.8    487.37
  2410    1822.8    462.37
  2410    1822.8    437.37
  2410    1822.8    412.37
  2410    1822.8    387.37
  2410    1822.8    362.37
  2410    1822.8    337.37
  2410    1822.8    312.37
  2410    1822.8    287.37
  2410    1822.8    262.37
  2410    1822.8    237.37
  2410    1822.8    212.37
  2410    1822.8    187.37
  2410    1822.8    162.37
  2410    1822.8    137.37
  2410    1822.8    112.37
  2410    1822.8    87.37
  2410    1822.8    62.37
  2410    1822.8    37.37
  2410    1822.8    12.37
  2410    1822.8    -12.63
  2410    1822.8    -37.63
  2410    1822.8    -62.63
  2410    1822.8    -87.63
  2410    1822.8    -112.63
  2410    1822.8    -137.63
  2410    1822.8    -162.63
  2410    1822.8    -187.63
  2410    1822.8    -212.63
  2410    1822.8    -237.63
  2410    1822.8    -262.63
  2410    1822.8    -287.63
  2410    1822.8    -312.63
  2410    1822.8    -337.63
  2410    1822.8    -362.63
  2410    1822.8    -387.63
  2410    1822.8    -412.63
  2410    1822.8    -437.63
  2410    1822.8    -462.63
  2410    1822.8    -487.63
  2410    1822.8    -512.63
  2410    1822.8    -537.63
  2410    1822.8    -562.63
  2410    1822.8    -587.63
  2410    1822.8    -612.63
  2410    1822.8    -637.63
  2410    1822.8    -662.63
  2410    1822.8    -687.63
  2410    1822.8    -712.63
  2410    1822.8    -737.63
  2410    1822.8    -762.63
  2410    1822.8    -787.63
  2410    1822.8    -812.63
  2410    1822.8    -837.63
  2410    1822.8    -862.63
  2410    1822.8    -887.63
  2410    1822.8    -912.63
  2410    1822.8    -915.62'
    sort_by = z
  []

  [56_32]
    type = PointValueSampler
    variable = 'temperature pressure disp_i disp_j disp_k'
    points = '1746.9    2137.1    1656.14
  1746.9    2137.2    1631.14
  1747    2137.2    1606.14
  1747    2137.3    1581.14
  1747.1    2137.4    1556.14
  1747.2    2137.7    1531.14
  1747.1    2138    1506.14
  1747    2138.3    1481.15
  1746.7    2138.6    1456.15
  1746.4    2138.5    1431.15
  1746.1    2139.1    1406.16
  1746    2139.7    1381.17
  1745.8    2140.3    1356.18
  1745.7    2141    1331.19
  1745.5    2141.7    1306.2
  1745.4    2142.4    1281.21
  1745.3    2143.2    1256.22
  1745.3    2144.1    1231.23
  1745.3    2144.9    1206.25
  1745.4    2145.5    1181.26
  1745.6    2146.1    1156.26
  1745.8    2146.8    1131.28
  1746.2    2147.5    1106.29
  1746.6    2148.2    1081.3
  1747    2148.6    1056.31
  1747.4    2149    1031.31
  1747.8    2149.4    1006.32
  1748.3    2149.8    981.33
  1748.7    2150.2    956.33
  1749.2    2150.6    931.34
  1749.6    2151    906.35
  1750.1    2151.3    881.36
  1750.8    2151.8    856.37
  1751.5    2152.2    831.38
  1752.3    2152.6    806.4
  1752.9    2153    781.41
  1753.3    2153.4    756.42
  1753.8    2153.9    731.43
  1754.3    2154.3    706.43
  1754.6    2154.3    681.44
  1755    2154.4    656.44
  1755.6    2154.4    631.45
  1756.2    2154.3    606.45
  1756.8    2154.2    581.46
  1757    2154.2    556.46
  1756.6    2154.1    531.46
  1756.2    2153.9    506.47
  1755.7    2153.6    481.48
  1755.1    2153.1    456.49
  1754.5    2152.5    431.5
  1754.4    2152.7    406.5
  1754.5    2153    381.51
  1754.4    2152.8    356.51
  1753.9    2152.4    331.52
  1753.3    2152.2    306.52
  1752.7    2152.6    281.54
  1751.5    2153    256.57
  1750.8    2152.6    231.58
  1750.8    2152.9    206.58
  1750.8    2153.3    181.59
  1750.7    2153.6    156.59
  1750.5    2153.8    131.59
  1750.2    2153.6    106.59
  1750.1    2153.2    81.6
  1750    2153.9    56.61
  1749.1    2153.9    31.63
  1748.2    2153.8    6.65
  1746.9    2153.4    -18.32
  1744.9    2152.4    -43.22
  1742.5    2151.1    -68.07
  1740.6    2149.7    -92.95
  1739.2    2148.6    -117.89
  1737.8    2147.5    -142.83
  1736.9    2146.1    -167.77
  1736.8    2144.6    -192.72
  1737    2143.3    -217.69
  1737.1    2142.5    -242.68
  1737    2142.1    -267.67
  1736.6    2141.5    -292.66
  1735.8    2140.7    -317.63
  1735.1    2139.8    -342.61
  1734.4    2139.3    -367.59
  1734    2139    -392.59
  1733.7    2138.5    -417.58
  1733.4    2137.8    -442.57
  1733    2136.9    -467.55
  1732.6    2136.1    -492.54
  1732.3    2135.5    -517.53
  1732.1    2135    -542.52
  1731.9    2134.5    -567.51
  1731.7    2133.8    -592.51
  1731.7    2133.1    -617.49
  1731.8    2132.2    -642.48
  1731.7    2131.1    -667.45
  1731.5    2129.6    -692.41
  1731.3    2128.2    -717.37
  1731.4    2127.1    -742.34
  1731.4    2125.9    -767.31
  1731    2124.7    -792.28
  1730.5    2124    -817.27
  1730.2    2123.8    -842.26
  1729.7    2123.8    -867.26
  1728.7    2123.8    -892.24
  1727.9    2124    -917.22
  1726.5    2124.2    -942.18
  1724.3    2124.2    -967.08
  1721.9    2124    -991.97
  1720.3    2123.6    -1016.91
  1720.5    2123    -1041.9
  1720.1    2122.5    -1066.88
  1719.5    2121.8    -1091.87
  1718.9    2121    -1116.84
  1718.5    2120.5    -1129.23'
    sort_by = z
  []
[]

############################################################
[Outputs]
  execute_on = 'timestep_end'
  csv = true
  file_base = FORGE_NS_Ph3_coarse_point_data
[]
############################################################
