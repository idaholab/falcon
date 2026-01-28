!include params_base.i
!include params_mdot.i

!include part_base.i
!include part_mdot.i
!include part_ics.i

frac_aperature = 1.5e-4
frac_roughness = 10e-3
inj_perm = 1e-9
inj_poro = 0.9
matrix_perm = 5.0e-17
matrix_poro = 2e-4

biot_coeff = 0.47

initial_dt = 100
dt_max = 50000

injection_temp = 323.15

!include part_frac_materials.i

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = porepressure
  temperature = temperature
  fp = fp_water
  pressure_unit = Pa
  stabilization = full
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'fracs_mesh.e'
  []
  construct_node_list_from_side_list = false
[]

[GlobalParams]
  PorousFlowDictator = dictator
  gravity = ${gravity_vector}
[]

[Variables]
  [porepressure]
  []
  [temperature]
    scaling = 1e-6
  []
[]

[AuxVariables]
  [density]
    order = CONSTANT
    family = MONOMIAL
  []
  [viscosity]
    order = CONSTANT
    family = MONOMIAL
  []
  [initial_p]
  []
  [permeability]
    order = CONSTANT
    family = MONOMIAL
  []
  [porosity]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
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
  [porosity]
    type = MaterialRealAux
    variable = porosity
    property = PorousFlow_porosity_qp
    execute_on = TIMESTEP_END
  []
  [permeability]
    type = MaterialRealTensorValueAux
    variable = permeability
    property = PorousFlow_permeability_qp
    execute_on = TIMESTEP_END
  []
[]

[AuxVariables]
  [porepressure_outflow]
  []
  [porepressure_out_zmax]
  []
  [temperature_outflow]
  []
[]

[Postprocessors]
  [porepressure_kg_per_s]
    type = NodalSum
    boundary = 'xmin xmax ymin ymax'
    variable = porepressure_outflow
  []
  [porepressure_zmax_kg_per_s]
    type = NodalSum
    boundary = 'zmax'
    variable = porepressure_out_zmax
  []
  [temperature_J_per_s]
    type = NodalSum
    boundary = 'xmin xmax ymin ymax'
    variable = temperature_outflow
  []
[]

[ICs]
  [porepressure]
    type = FunctionIC
    variable = porepressure
    function = initial_p_fn
  []
  [temperature]
    type = FunctionIC
    variable = temperature
    function = insitu_T
  []
  [initial_p_ic]
    type = FunctionIC
    variable = initial_p
    function = initial_p_fn
  []
[]

[Materials]
  [biot_modulus]
    type = PorousFlowConstantBiotModulus
    biot_coefficient = ${biot_coeff}
    solid_bulk_compliance = 2e-7
    fluid_bulk_modulus = 1e7
  []

  [porosity_inj_prod_volume]
    type = PorousFlowPorosityConst
    porosity = ${inj_poro}
    block = '2000 3000'
  []
  [permeability_inj_prod_volume]
    type = PorousFlowPermeabilityConst
    block = '2000 3000'
    permeability = '${inj_perm} 0 0 0 ${inj_perm} 0 0 0 ${inj_perm}'
  []
  [rock_internal_energy_inj_prod_volume]
    type = PorousFlowMatrixInternalEnergy
    density = 2500.0
    specific_heat_capacity = 100.0
    block = '2000 3000'
  []
  [thermal_conductivity_inj_prod_volume]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '3 0 0 0 3 0 0 0 3'
    block = '2000 3000'
  []

  [porosity_matrix]
    type = PorousFlowPorosity
    porosity_zero = ${matrix_poro}
    block = '1000'
  []
  [permeability_matrix]
    type = PorousFlowPermeabilityConst
    permeability = '${matrix_perm} 0 0  0 ${matrix_perm} 0  0 0 ${matrix_perm}'
    block = '1000'
  []
  [rock_internal_energy_matrix]
    type = PorousFlowMatrixInternalEnergy
    density = 2750.0
    specific_heat_capacity = 790.0
    block = '1000'
  []
  [thermal_conductivity_matrix]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '3.05 0 0 0 3.05 0 0 0 3.05'
    block = '1000'
  []
[]

[Postprocessors]
  [inject_T]
    type = Receiver
    default = ${injection_temp}
  []
  [param_frac_aperature]
    type = Receiver
    default = '${frac_aperature}'
  []
  [param_frac_roughness]
    type = Receiver
    default = ${frac_roughness}
  []
  [param_matrix_perm]
    type = Receiver
    default = ${matrix_perm}
  []
  [param_matrix_poro]
    type = Receiver
    default = ${matrix_poro}
  []
  [param_biot_coeff]
    type = Receiver
    default = ${biot_coeff}
  []
  [injection_rate_kg_s]
    type = FunctionValuePostprocessor
    function = inlet_mdot_fn
    execute_on = timestep_end
  []
  [a2_nl_it]
    type = NumNonlinearIterations
  []
  [a1_dt]
    type = TimestepSize
  []
  [a0_wall_time]
    type = PerfGraphData
    section_name = "Root"
    data_type = total
  []
[]

[Preconditioning]
  active = asm_ilu
  [asm_ilu]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -ksp_grmres_restart -pc_type -sub_pc_type -sub_pc_factor_shift_type -pc_asm_overlap'
    petsc_options_value = 'gmres 30 asm ilu NONZERO 2'
  []
[]

[Executioner]
  type = Transient

  start_time = ${initial_time}
  end_time = ${final_time}

  dtmin = 1e-3
  dtmax = ${dt_max}
  [TimeStepper]
    type = IterationAdaptiveDT
    optimal_iterations = 6
    iteration_window = 0
    growth_factor = 1.2
    cutback_factor = 0.9
    cutback_factor_at_failure = 0.5
    linear_iteration_ratio = 1000
    dt = ${initial_dt}
  []
  error_on_dtmin = false

  solve_type = NEWTON
  line_search = none
  nl_max_its = 20
  nl_abs_tol = 1e-5
  nl_rel_tol = 1e-5
  l_tol = 1e-4
  l_max_its = 200
  automatic_scaling = true
[]

[Outputs]
  csv = true
  exodus = true
  print_linear_residuals = false
  [console]
    type = Console
    execute_postprocessors_on = 'NONE'
  []
[]
