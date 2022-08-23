#---geothermal battery project - looking for sweet spot
#---Define two scalar variables to sweep consistently

depth =  2000 
pp_ini_bc = ${fparse if(depth<=2000, 1000*9.8*depth, 1000*9.8*depth+(depth-2000)*9.8*1000)}
T_ini_bc  = ${fparse (80+3.28084*depth*1.1/100-32)*5/9 + 273.15}

distance_between_wells = 200
pro_well_x = ${fparse 0.1 + distance_between_wells / 2}
inj_well_x  = ${fparse 0.1 - distance_between_wells / 2}
well_length = 50  #fixed number in accordance with the mesh
aquifer_mid= ${fparse 0 + 0}
aquifer_top= ${fparse 0 + well_length/2}
aquifer_bot= ${fparse 0 - well_length/2}
#---Define a scalar variable to sweep 
inj_ext_exponent = 1.5
inj_ext_flux= ${fparse 10^inj_ext_exponent / well_length / 2 }

injection_temp = 473.15

#---Define a scalar variable to replace the tensor components
Tcond_aquifer = 2.0
#---Define a scalar variable to replace the tensor components
perm_exponent = -12
perm_aquifer = ${fparse 10^perm_exponent}

[Controls]
  [summer_injection]
    type = ConditionalFunctionEnableControl
    conditional_function = inj_function_summer
    enable_objects  = 'DiracKernel::charge_injection_P DiracKernel::charge_injection_T DiracKernel::charge_production_P DiracKernel::charge_production_T'
  []
  [winter_injection]
    type = ConditionalFunctionEnableControl
    conditional_function = inj_function_winter
    enable_objects  = 'DiracKernel::recover_injection_P DiracKernel::recover_injection_T DiracKernel::recover_production_P DiracKernel::recover_production_T'
  []
  [rest_injection]
    type = ConditionalFunctionEnableControl
    conditional_function = rest_function
    enable_objects  = 'DiracKernel::rest_injection_P DiracKernel::rest_injection_T DiracKernel::rest_production_P DiracKernel::rest_production_T'
  []
[]

[Functions]
  [./inj_function_summer]
    type = ParsedFunction
    value = '(t/24/3600/365.25-floor(t/24/3600/365.25))>0 & (t/24/3600/365.25-floor(t/24/3600/365.25))<=0.25'
  [../]
  [./inj_function_winter]
    type = ParsedFunction
    vals = 'termination'
    vars = 'b_switch'
    value = '((t/24/3600/365.25-floor(t/24/3600/365.25))>0.5 & (t/24/3600/365.25-floor(t/24/3600/365.25)) <=0.75) & b_switch <= 0'
  [../]
  [./rest_function]
    type = ParsedFunction
    vals = 'termination'
    vars = 'b_switch'
    value = '(((t/24/3600/365.25-floor(t/24/3600/365.25))>0.25 & (t/24/3600/365.25-floor(t/24/3600/365.25)) <=0.5) | ((t/24/3600/365.25-floor(t/24/3600/365.25))>0.75 & (t/24/3600/365.25-floor(t/24/3600/365.25)) <=1)) | (((t/24/3600/365.25-floor(t/24/3600/365.25))>0.5 & (t/24/3600/365.25-floor(t/24/3600/365.25)) <=0.75) & b_switch >=1)'
  [../]
  [./Fiss_Function]
    type = PiecewiseLinear
    x ='0.            7889400.      15778800.     23668200.
      31557600.     39447000.     47336400.     55225800.
      63115200.     71004600.     78894000.     86783400.
      94672800.     102562200.    110451600.    118341000.
      126230400.    134119800.    142009200.    149898600.
      157788000.    165677400.    173566800.    181456200.
      189345600.    197235000.    205124400.    213013800.
      220903200.    228792600.    236682000.    244571400.
      252460800.    260350200.    268239600.    276129000.
      284018400.    291907800.    299797200.    307686600.
      315576000.'
    y = '0 1  2  3  4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4'
  [../]
[]

# Darcy flow with heat advection and conduction
[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = base_final_50.e
  []
[]
#############################################################
[GlobalParams]
  PorousFlowDictator = dictator
[]
############################################################
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
    scaling = 1E-6
  [../]
[]
###########################################################
[AuxVariables]
  [./density]
    order = CONSTANT
    family = MONOMIAL
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
[BCs]
  [./P_drained]
    type = DirichletBC
    variable = porepressure
    boundary = 'east west north'
    value = ${pp_ini_bc}
  [../]
  [./T_cont]
    type = DirichletBC
    variable = temperature
    boundary = 'east west north'
    value = ${T_ini_bc}
  [../]
[]

############################################################
[DiracKernels]
  [./charge_injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 ${inj_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'  # ~5 kg/s over length of 10(injection_length)/2
  [../]
  [./charge_injection_T]
    type = EnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 ${inj_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    T_in = ${injection_temp}
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./charge_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 ${pro_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]
  [./charge_production_T]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 ${pro_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]

  [./recover_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 ${inj_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'  # ~5 kg/s over length of 10(injection_length)/2
  [../]
  [./recover_production_T]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 ${inj_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]
  [./recover_injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 ${pro_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./recover_injection_T]
    type = EnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 ${pro_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    T_in = ${T_ini_bc}
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]

[./rest_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 ${inj_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = '-1e9 1e9'
    fluxes = '0 0'  # ~5 kg/s over length of 10(injection_length)/2
  [../]
  [./rest_production_T]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 ${inj_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '0 0'
  [../]
  [./rest_injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 ${pro_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = '-1e9 1e9'
    fluxes = '0 0'
  [../]
  [./rest_injection_T]
    type = EnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 ${pro_well_x} 0 ${aquifer_bot}'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    T_in = ${T_ini_bc}
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '0 0'
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
    specific_heat_capacity = 930.0
    density = 2650.0
    block = 'aquifer_HEX8 aquifer_WEDGE'
  [../]
  [./thermal_conductivity_aquifer]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '${Tcond_aquifer} 0 0  0 ${Tcond_aquifer} 0  0 0 ${Tcond_aquifer}'
    block = 'aquifer_HEX8 aquifer_WEDGE'
  [../]
  [./porosity_aquifer]
    type = PorousFlowPorosityConst
    block = 'aquifer_HEX8 aquifer_WEDGE'
    porosity = 0.01 
  [../]
  [./permeability_aquifer]
    type = PorousFlowPermeabilityConst
    block = 'aquifer_HEX8 aquifer_WEDGE'
    permeability = '${perm_aquifer} 0 0   0 ${perm_aquifer} 0   0 0 ${perm_aquifer}'
  [../]
  [./internal_energy_caps]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 1000.0
    density = 2500.0
    block = 'caps_HEX8 caps_WEDGE'
  [../]
  [./thermal_conductivity_caps]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.5 0 0  0 2.5 0  0 0 2.5'
    block = 'caps_HEX8 caps_WEDGE'
  [../]
  [./porosity_caps]
    type = PorousFlowPorosityConst
    block = 'caps_HEX8 caps_WEDGE'
    porosity = 0.01
  [../]
  [./permeability_caps]
    type = PorousFlowPermeabilityConst
    block = 'caps_HEX8 caps_WEDGE'
    permeability = '1E-18 0 0   0 1E-18 0   0 0 1E-18'
  [../]
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

############################################################

[Postprocessors]
  [./hotwell_mass_kg]
    type = PorousFlowPlotQuantity
    uo = fluid_mass_in_inc
  [../]
  [./hotwell_enthalpy_J]
    type = PorousFlowPlotQuantity
    uo = heat_enthalpy_in_inc
  [../]
  [./coldwell_mass_kg]
    type = PorousFlowPlotQuantity
    uo = fluid_mass_out_inc
  [../]
  [./coldwell_enthalpy_J]
    type = PorousFlowPlotQuantity
    uo = heat_enthalpy_out_inc
  [../]

  [./time]
    type = TimePostprocessor
    execute_on = 'timestep_end'
  []
  [./step_dt]
    type = TimestepSize
  [../]

  [./is_winter]
    type = FunctionValuePostprocessor
    function = inj_function_winter
  [../]
  [./is_summer]
    type = FunctionValuePostprocessor
    function = inj_function_summer
  [../]


  [./total_recovery_time]
    type = PorousFlowRecoveryTimeAccumulator
    targetpostprocessor = is_winter
    dtpostprocessor = step_dt
  [../]

  [./termination]
    type = PorousFlowTemperatureDropTerminator
    enthalpypostprocessor = hotwell_enthalpy_J
    masspostprocessor = hotwell_mass_kg
    T_inj = ${injection_temp}
    T_init = ${T_ini_bc}
    P_drop = 80
    timepostprocessor = time 
  [../]

  [./recovery_rate]
    type = PorousFlowRecoveryRateSeason
    hotwellenergy = hotwell_enthalpy_J
    coldwellenergy = coldwell_enthalpy_J
    InjectionIndicator = is_summer
    ProductionIndicator = is_winter
  [../]

  [./total_energy]
    type = PorousFlowEnergyAccumulator
    hotwellenergy = hotwell_enthalpy_J
    coldwellenergy = coldwell_enthalpy_J
    ProductionIndicator = is_winter
  [../]

  [./inj_P_mid]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${inj_well_x} 0 ${aquifer_mid}'
    variable = porepressure
  [../]
  [./inj_T_mid]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${inj_well_x} 0 ${aquifer_mid}'
    variable = temperature
  [../]
  [./pro_P_mid]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${pro_well_x} 0 ${aquifer_mid}'
    variable = porepressure
  [../]
  [./pro_T_mid]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${pro_well_x} 0 ${aquifer_mid}'
    variable = temperature
  [../]
[] 

##########################################################
[Executioner]
  type = Transient
  solve_type = NEWTON
  end_time = 315576000
#  steady_state_detection = true
#  steady_state_start_time = 864000 # 10 days
#  steady_state_tolerance = 1e-6
  dtmax = 864000 # 10 days
  dtmin = 100
 [./TimeStepper]
   type = IterationAdaptiveDT
   dt = 1000.0
   optimal_iterations = 10
   force_step_every_function_point =  true
   timestep_limiting_function = Fiss_Function
#  timestep_limiting_postprocessor = timestep_pp
 [../]
# controls for linear iterations
  l_max_its = 50  
  l_tol = 1e-3
# controls for nonlinear iterations
  nl_max_its = 20
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-4
[]
###########################################################
[Outputs]



#[./exodus]
#    type = Exodus
#    file_base = sequence_out_1
#    elemental_as_nodal = true
#    overwrite = true
#    sequence = true
#     interval = 2
#[../]

#[./Console]
#  type = Console
#  #perf_log = false
#  #output_linear = true
#  output_nonlinear = true
#  interval = 1
#[../]

[./CSV]
  type = CSV
  interval = 1
[../]
[]
###########################################################
