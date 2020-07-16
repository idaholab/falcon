#---geothermal battery project - looking for sweet spot
#---sweep parameters/ICs/BCs:
# ICs/init_pp/porepressure=[1e7 2e7 3e7 4e7]
# Ics/init_temp/temperature = [293.15 313.15 333.15 350.15]
#----Note the two BCs in the following need to be adjusted according to the ICs
# BCs/P_drained/function =[1e7 2e7 3e7 4e7]
# BCs/T_cont/function = [293.15 313.15 333.15 350.15]
#---Define two scalar variables to sweep consistently
pp_ini_bc = 1e7
T_ini_bc = 293.15

# DiracKernel/injection_T/T_in = [373.1500  398.1500  423.1500  448.1500  473.1500  498.1500  523.1500  548.1500  573.1500]

#----The following two need to be activiated at injection period
# DiracKernels/injection_T/fluxes = [0.25 0.5 1.25 2.5]
# DiracKernels/injection_P/fluxes = [0.25 0.5 1.25 2.5]
#----The following two need to be activiated at extraction period
# DiracKernels/production_P/fluxes = [0.25 0.5 1.25 2.5]
# DiracKernels/production_T/fluxes = [0.25 0.5 1.25 2.5]
#---Define a scalar variable to sweep consistently
inj_ext_flux= 0.25

# Materials/thermal_conductivity_aquifer/dry_thermal_conductivity = [2 2.5 3]
#---Define a scalar variable to replace the tensor components
Tcond_aquifer = 2

# Materials/porosity_aquifer/porosity = [0.01 0.05 0.1 0.2 0.3]

# Materials/permeability_aquifer/permeability = [1e-12 1e-13 1e-14 1e-15 1e-16 1e-17]
#---Define a scalar variable to replace the tensor components
perm_aquifer = 1e-12

#---Define a scalar variable to automatically change injection duration
switch_to_extraction = 7776000
full_duration = ${fparse 2 * switch_to_extraction}

#[Controls]
#  [extraction-period]
#    type = TimePeriod
#    enable_objects = 'DiracKernel::production_P DiracKernel::production_T'
#    start_time = '${switch_to_extraction}'
#    end_time = '${full_duration}'
#    set_sync_times = true
#  []
#  [injection-period]
#    type = TimePeriod
#    enable_objects =  'DiracKernel::injection_T DiracKernel::injection_P'
#    start_time = '0'
#    end_time = '${switch_to_extraction}'
#    set_sync_times = true
#  []
#[]

# Darcy flow with heat advection and conduction
[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = doublet_10-10.e
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
    type = FunctionPresetBC
    variable = porepressure
    boundary = 'east west north'
    function = ${pp_ini_bc}
  [../]
  [./T_cont]
    type = FunctionPresetBC
    variable = temperature
    boundary = 'east west north'
    function = ${T_ini_bc}
  [../]
[]

distance_between_wells = 20
pro_well_x = ${fparse 0 + distance_between_wells / 2}
inj_well_x  = ${fparse 0 - distance_between_wells / 2}
well_length = 10  #fixed number in accordance with the mesh
############################################################
[DiracKernels]
  [./injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 ${inj_well_x} 0 10'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'  # ~5 kg/s over length of 10(injection_length)/2
  [../]
  [./injection_T]
    type = PorousFlowEnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 ${inj_well_x} 0 10'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    T_in = 373.15
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 ${pro_well_x} 0 10'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]
  [./production_T]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 ${pro_well_x} 0 10'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
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
    block = aquifer
  [../]

  [./thermal_conductivity_aquifer]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '${Tcond_aquifer} 0 0  0 ${Tcond_aquifer} 0  0 0 ${Tcond_aquifer}'
    block = aquifer
  [../]

  [./porosity_aquifer]
    type = PorousFlowPorosityConst
    block = aquifer
    porosity = 0.01 
  [../]

  [./permeability_aquifer]
    type = PorousFlowPermeabilityConst
    block = aquifer
    permeability = '${perm_aquifer} 0 0   0 ${perm_aquifer} 0   0 0 ${perm_aquifer}'
  [../]


  [./internal_energy_caps]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 770.0
    density = 2500.0
    block = caps
  [../]
  [./thermal_conductivity_caps]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.5 0 0  0 2.5 0  0 0 2.5'
    block = caps
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
[]
############################################################
[Preconditioning]
#active = 'ilu_may_use_less_mem'
active = 'superlu'

[./ilu_may_use_less_mem]
type = SMP
full = true
petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type'
petsc_options_value = 'gmres asm ilu NONZERO'
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
aquifer_mid= ${fparse 10 + well_length / 2}
aquifer_top= ${fparse 10 + well_length}
[Postprocessors]
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

  [./inj_P_mid]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${inj_well_x} 0 ${aquifer_mid}'
    variable = porepressure
  [./inj_T_mid]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${inj_well_x} 0 ${aquifer_mid}'
    variable = temperature
  [../]

  [./pro_P_bot]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${pro_well_x} 0 10'
    variable = porepressure
  [../]
  [./pro_T_bot]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${pro_well_x} 0 10'
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

  [./pro_P_top]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${pro_well_x} 0 ${aquifer_top}'
    variable = porepressure
  [../]
  [./pro_T_top]
    type = PointValue
    execute_on = 'initial timestep_end'
    point = '${pro_well_x} 0 ${aquifer_top}'
    variable = temperature
  [../]

#  [./pro_mean_temp]
#    type = FunctionValuePostprocessor
#    function = temp_mean_fcn
#    execute_on = timestep_end
#  [../]
[] 

#[VectorPostprocessors]
#  [./pp]
#    type = LineValueSampler
#    num_points = 10
#    start_point = '${pro_well_x} 0 10'
#    end_point = '${pro_well_x} 0 ${aquifer_top}'
#    sort_by = z
#    variable = temperature
#  [../]
#  [stats]
#    type = Statistics
#    vectorpostprocessors = 'pp'
#    compute = 'mean'
#  []
#[]

#[Functions]
#  [./temp_mean_fcn]
#    type = ParsedFunction
#    value = abs(a)
#    vars = 'a'
#    vals = 'pro_mean_temp'
#  [../]
#[]

##########################################################
[Executioner]
  type = Transient
  solve_type = NEWTON
  end_time = 86400 #${full_duration}
  dtmax = 7200 #2 hours
  dtmin = 100
 [./TimeStepper]
   type = IterationAdaptiveDT
   dt = 1000.0
   optimal_iterations = 5
 [../]
# controls for linear iterations
  l_max_its = 15
  l_tol = 1e-4
# controls for nonlinear iterations
  nl_max_its = 20
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-4
[]
###########################################################
[Outputs]

[./exodus]
    type = Exodus
#    file_base = sequence_out_1
    elemental_as_nodal = true
    overwrite = true
#    sequence = true
[../]

[./Console]
  type = Console
  #perf_log = false
  output_linear = true
  output_nonlinear = true
  interval = 1
[../]

[./CSV]
  type = CSV
  interval = 1
[../]
[]
###########################################################
