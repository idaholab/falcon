#---geothermal battery project - Cranfield - cross validation with TOUGH simulator
#---define initial pressure and temperature
pp_ini_bc = 3e+7   # Pa
T_ini_bc = 399.15   # K

well_length = 15.6  #lower two-thirds of the formation
inj_ext_flux= ${fparse 3/well_length/4 } # 3 kg/s over injection length with 1/4 sysmentrical model

# Darcy flow with heat advection and conduction
[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = Cranfield_caps_old_coarse.e
  []
[]
#############################################################
[GlobalParams]
  PorousFlowDictator = dictator
[]
####################################################‰########
[PorousFlowUnsaturated]
  relative_permeability_type = COREY
  relative_permeability_exponent = 0
  add_darcy_aux = true
  coupling_type = ThermoHydro
  gravity = '0 0 -9.8'
  porepressure = porepressure
  temperature = temperature
  fp = tabulated_water
  use_displaced_mesh = false
[]
###########################################################
[Variables]
  [./porepressure]
  [../]
  [./temperature]
    scaling = 1E-8
  [../]
[]
###########################################################
[AuxVariables]
  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./perm_z]
    family = monomial
    order = constant
  [../]
  [./perm_x]
    family = monomial
    order = constant
  [../]
[]
##############################################################
[AuxKernels]
  [./density]
    type = MaterialRealAux
    variable = density
    property = PorousFlow_fluid_phase_density_qp0
    execute_on = TIMESTEP_END
  [../]
  [./perm_z]
    type = PorousFlowPropertyAux
    property = permeability
    variable = perm_z
    row = 2
    column = 2
  [../]
  [./perm_x]
    type = PorousFlowPropertyAux
    property = permeability
    variable = perm_x
    row = 1
    column = 1
  [../]
[]
############################################################
[UserObjects]
  [./fluid_mass_in_inc]
    type = PorousFlowSumQuantity
  [../]
  [./fluid_mass_out_inc]
    type = PorousFlowSumQuantity
  [../]
  [./heat_enthalpy_in_inc]
    type = PorousFlowSumQuantity
  [../]
  [./heat_enthalpy_out_inc]
    type = PorousFlowSumQuantity
  [../]
[]
############################################################
[ICs]
  [./init_pp]
    type = FunctionIC
    function = ${pp_ini_bc}
    variable = porepressure
  [../]
  [./init_temp]
    type = ConstantIC
    value = ${T_ini_bc}
    variable = temperature
  [../]
[]
###############################################################
#[BCs]
#  [./P_drained]
#    type = DirichletBC
#    variable = porepressure
#    boundary = 'top bottom'
#    value = ${pp_ini_bc}
#  [../]
#  [./T_cont]
#    type = DirichletBC
#    variable = temperature
#    boundary = 'top bottom'
#    value = ${T_ini_bc}
#  [../]
#[]

############################################################
[DiracKernels]
  [./summer_injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 -9.99 -9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./summer_injection_T]
    type = EnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 -9.99 -9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    T_in = 453.15
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./summer_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 9.99 9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]
  [./summer_production_T]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 9.99 9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]
   [./winter_injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 -9.99 -9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./winter_injection_T]
    type = EnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 -9.99 -9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    T_in = 399.15
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./winter_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 9.99 9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]
  [./winter_production_T]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 9.99 9.99 -10.8'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]

[]
############################################################
[Controls]
  [summer_injection]
    type = ConditionalFunctionEnableControl
    conditional_function = inj_function_summer
    enable_objects  = 'DiracKernels::summer_injection_P DiracKernels::summer_injection_T DiracKernels::summer_production_P DiracKernels::summer_production_T'
  []
  [winter_injection]
    type = ConditionalFunctionEnableControl
    conditional_function = inj_function_winter
    enable_objects  = 'DiracKernels::winter_injection_P DiracKernels::winter_injection_T DiracKernels::winter_production_P DiracKernels::winter_production_T'
  [] 
[]

[Functions]
  [./inj_function_summer]
    type = ParsedFunction
    value = '(t/24/3600/365-floor(t/24/3600/365))<=0.25'
  [../]
  [./inj_function_winter]
    type = ParsedFunction
    value = '(t/24/3600/365-floor(t/24/3600/365))>0.5 & (t/24/3600/365-floor(t/24/3600/365)) <=0.75'
  [../]
[]

############################################################
[Modules]
  [./FluidProperties]
    [./true_water]
      type = Water97FluidProperties
    [../]
    [./tabulated_water]
      type = TabulatedFluidProperties
      fp = true_water
      temperature_min = 275
      temperature_max = 600
      pressure_max = 1E8
      interpolated_properties = 'density viscosity enthalpy internal_energy'
      fluid_property_file = water97_tabulated.csv
    [../]
  [../]
[]
############################################################
[Materials]
  [./internal_energy_aquifer]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 920.0 
    density = 2600.0
    block = 'caps layer_1 layer_2 layer_3 layer_4 layer_5 layer_6 layer_7 layer_8 layer_9 layer_10 layer_11 layer_12 layer_13 layer_14 layer_15 layer_16 layer_17 layer_18 layer_19 layer_20'
  [../]
  [./thermal_conductivity_aquifer]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.51 0 0  0 2.51 0  0 0 2.51'
    block = 'caps layer_1 layer_2 layer_3 layer_4 layer_5 layer_6 layer_7 layer_8 layer_9 layer_10 layer_11 layer_12 layer_13 layer_14 layer_15 layer_16 layer_17 layer_18 layer_19 layer_20'
  [../]

  [./porosity_caps]
    type = PorousFlowPorosityConst
    block = caps
    porosity = 0.01
  [../]
  [./permeability_caps]
    type = PorousFlowPermeabilityConst
    block = caps
    permeability = '1E-18 0 0   0 1E-18 0   0 0 1E-18'
  [../]

  [./porosity_layer_1]
    type = PorousFlowPorosity
    block = layer_1
    porosity_zero = 0.143
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8  
  [../]
  [./permeability_layer_1]
    type = PorousFlowPermeabilityConst
    block = layer_1
    permeability = '.8130E-15 0 0   0 .8130E-15 0   0 0 .3160E-15'
  [../]

  [./porosity_layer_2]
    type = PorousFlowPorosity
    block = layer_2
    porosity_zero = 0.158
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8  
  [../]
  [./permeability_layer_2]
    type = PorousFlowPermeabilityConst
    block = layer_2
    permeability = '.3460E-14 0 0   0 .3460E-14 0   0 0 .1350E-14'
  [../]

  [./porosity_layer_3]
    type = PorousFlowPorosity
    block = layer_3
    porosity_zero = 0.176
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8  
  [../]
  [./permeability_layer_3]
    type = PorousFlowPermeabilityConst
    block = layer_3
    permeability = '.1120E-13 0 0   0 .1120E-13 0   0 0 .4390E-14'
  [../]

  [./porosity_layer_4]
    type = PorousFlowPorosity
    block = layer_4
    porosity_zero = 0.188
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_4]
    type = PorousFlowPermeabilityConst
    block = layer_4
    permeability = '.1920E-13 0 0   0 .1920E-13 0   0 0 .1400E-13'
  [../]

  [./porosity_layer_5]
    type = PorousFlowPorosity
    block = layer_5
    porosity_zero = 0.166
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_5]
    type = PorousFlowPermeabilityConst
    block = layer_5
    permeability = '.5500E-14 0 0   0 .5500E-14 0   0 0 .3900E-14'
  [../]

  [./porosity_layer_6]
    type = PorousFlowPorosity
    block = layer_6
    porosity_zero = 0.185
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_6]
    type = PorousFlowPermeabilityConst
    block = layer_6
    permeability = '.1770E-13 0 0   0 .1770E-13 0   0 0 .1190E-13'
  [../]

  [./porosity_layer_7]
    type = PorousFlowPorosity
    block = layer_7
    porosity_zero = 0.24
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_7]
    type = PorousFlowPermeabilityConst
    block = layer_7
    permeability = '.9110E-13 0 0   0 .9110E-13 0   0 0 .8870E-13'
  [../]

  [./porosity_layer_8]
    type = PorousFlowPorosity
    block = layer_8
    porosity_zero = 0.26
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_8]
    type = PorousFlowPermeabilityConst
    block = layer_8
    permeability = '.1390E-12 0 0   0 .1390E-12 0   0 0 .1290E-12'
  [../]

  [./porosity_layer_9]
    type = PorousFlowPorosity
    block = layer_9
    porosity_zero = 0.285
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_9]
    type = PorousFlowPermeabilityConst
    block = layer_9
    permeability = '.2120E-12 0 0   0 .2120E-12 0   0 0 .2000E-12'
  [../]

  [./porosity_layer_10]
    type = PorousFlowPorosity
    block = layer_10
    porosity_zero = 0.27
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_10]
    type = PorousFlowPermeabilityConst
    block = layer_10
    permeability = '.1630E-12 0 0   0 .1630E-12 0   0 0 .1580E-12'
  [../]

  [./porosity_layer_11]
    type = PorousFlowPorosity
    block = layer_11
    porosity_zero = 0.23
  [../]
  [./permeability_layer_11]
    type = PorousFlowPermeabilityConst
    block = layer_11
    permeability = '.7700E-13 0 0   0 .7700E-13 0   0 0 .5990E-13'
  [../]

  [./porosity_layer_12]
    type = PorousFlowPorosity
    block = layer_12
    porosity_zero = 0.306
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_12]
    type = PorousFlowPermeabilityConst
    block = layer_12
    permeability = '.2880E-12 0 0   0 .2880E-12 0   0 0 .2880E-12'
  [../]

  [./porosity_layer_13]
    type = PorousFlowPorosity
    block = layer_13
    porosity_zero = .285
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_13]
    type = PorousFlowPermeabilityConst
    block = layer_13
    permeability = '.2130E-12 0 0   0 .2130E-12 0   0 0 .2040E-12'
  [../]

  [./porosity_layer_14]
    type = PorousFlowPorosity
    block = layer_14
    porosity_zero = .155
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_14]
    type = PorousFlowPermeabilityConst
    block = layer_14
    permeability = '.1080E-13 0 0   0 .1080E-13 0   0 0 .2190E-15'
  [../]

  [./porosity_layer_15]
    type = PorousFlowPorosity
    block = layer_15
    porosity_zero = .299
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_15]
    type = PorousFlowPermeabilityConst
    block = layer_15
    permeability = '.2660E-12 0 0   0 .2660E-12 0   0 0 .2330E-12'
  [../]

  [./porosity_layer_16]
    type = PorousFlowPorosity
    block = layer_16
    porosity_zero = .334
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_16]
    type = PorousFlowPermeabilityConst
    block = layer_16
    permeability = '.4230E-12 0 0   0 .4230E-12 0   0 0 .4110E-12'
  [../]

  [./porosity_layer_17]
    type = PorousFlowPorosity
    block = layer_17
    porosity_zero = .32
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_17]
    type = PorousFlowPermeabilityConst
    block = layer_17
    permeability = '.3560E-12 0 0   0 .3560E-12 0   0 0 .3300E-12'
  [../]

  [./porosity_layer_18]
    type = PorousFlowPorosity
    block = layer_18
    porosity_zero = .294
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_18]
    type = PorousFlowPermeabilityConst
    block = layer_18
    permeability = '.2420E-12 0 0   0 .2420E-12 0   0 0 .2360E-12'
  [../]

  [./porosity_layer_19]
    type = PorousFlowPorosity
    block = layer_19
    porosity_zero = .279
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_19]
    type = PorousFlowPermeabilityConst
    block = layer_19
    permeability = '.2050E-12 0 0   0 .2050E-12 0   0 0 .1310E-12'
  [../]

  [./porosity_layer_20]
    type = PorousFlowPorosity
    block = layer_20
    porosity_zero = .071
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
  [../]
  [./permeability_layer_20]
    type = PorousFlowPermeabilityConst
    block = layer_20
    permeability = '.2700E-16 0 0   0 .2700E-16 0   0 0 .1000E-17 '
  [../]
[]

############################################################

[Postprocessors]
  [./is_summer]
    type = FunctionValuePostprocessor
    function = inj_function_summer
  [../]				
  [./is_winter]
    type = FunctionValuePostprocessor
    function = inj_function_winter
  [../]
  [./inlet_mass_kg]
    type = PorousFlowPlotQuantity
    uo = fluid_mass_in_inc
  [../]
  [./inlet_enthalpy_J]
    type = PorousFlowPlotQuantity
    uo = heat_enthalpy_in_inc
  [../]
  [./outlet_mass_kg]
    type = PorousFlowPlotQuantity
    uo = fluid_mass_out_inc
  [../]
  [./outlet_enthalpy_J]
    type = PorousFlowPlotQuantity
    uo = heat_enthalpy_out_inc
  [../]

  [./production_temperature]
    type = AverageNodalVariableValue
    boundary = extraction
    variable = temperature
  [../]
  [./production_porepressure]
    type = AverageNodalVariableValue
    boundary = extraction
    variable = porepressure
  [../]
  [./injection_temperature]
    type = AverageNodalVariableValue
    boundary = injection
    variable = temperature
  [../]
  [./injection_porepressure]
    type = AverageNodalVariableValue
    boundary = injection
    variable = porepressure
  [../]
  [total]
    type = MemoryUsage
    mem_units = 'bytes'
    execute_on = 'INITIAL TIMESTEP_END'
  []
[] 

############################################################
[Preconditioning]
active = 'ilu_may_use_less_mem'
#active = 'superlu'

[./ilu_may_use_less_mem]
type = SMP
full = true
petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type -sub_pc_factor_levels'
petsc_options_value = 'gmres asm ilu NONZERO 2'
[../]

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
#per Andy
type = SMP
full = true
petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
petsc_options_value = ' lu       mumps'
[../]

[./basic]
type = SMP
full = true
petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type -pc_asm_overlap'
petsc_options_value = ' asm      lu           NONZERO                   2'
[../]

[]

##########################################################
[Executioner]
  type = Transient
  solve_type = NEWTON
  end_time = 31635000
#  steady_state_detection = true
#  steady_state_start_time = 864000 # 10 days
#  steady_state_tolerance = 1e-6
  dtmax = 216000 # 2 days
  dtmin = 10
 [./TimeStepper]
   type = IterationAdaptiveDT
   dt = 1000.0
   optimal_iterations = 10
 [../]
# controls for linear iterations
  l_max_its = 100
  l_tol = 1e-4
# controls for nonlinear iterations
  nl_max_its = 50
  nl_abs_tol = 1e-5
[]
###########################################################
[Outputs]

[./exodus]
    type = Exodus
#    file_base = sequence_out_1
#    elemental_as_nodal = true
#    overwrite = true
#    sequence = true
    interval = 20
[../]

[./Console]
  type = Console
  #perf_log = false
  #output_linear = false
  output_nonlinear = true
  interval = 1
[../]

[./CSV]
  type = CSV
  interval = 1
[../]
[]
###########################################################
