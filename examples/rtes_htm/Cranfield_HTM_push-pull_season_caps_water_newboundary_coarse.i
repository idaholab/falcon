#---geothermal battery project - Cranfield - cross validation with TOUGH simulator
#---define initial pressure and temperature
pp_ini_bc = 3e+7   # Pa
T_ini_bc = 399.15   # K
sigmaV_ini_bc = 67.3E6   # Pa
sigmaH_ini_bc = 42.9E6   # Pa

well_length = 1.2  #lower two-thirds of the formation
inj_ext_flux= ${fparse 3/well_length/4 } # 3 kg/s over injection length with 1/4 sysmentrical model

# Darcy flow with heat advection and conduction
[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = Cranfield_caps_new_coarse.e
  []
[]
#############################################################
[GlobalParams]
  PorousFlowDictator = dictator
  displacements = 'disp_x disp_y disp_z'
[]
####################################################‰########
[PorousFlowUnsaturated]
  relative_permeability_type = COREY
  relative_permeability_exponent = 0
  add_darcy_aux = true
  coupling_type = ThermoHydroMechanical
  gravity = '0 0 -9.8'
  porepressure = porepressure
  temperature = temperature
  thermal_eigenstrain_name = thermal_contribution
  fp = tabulated_water
  use_displaced_mesh = true
  biot_coefficient  = 0.9
[]
###########################################################
[Variables]
  [./porepressure]
  [../]
  [./temperature]
    scaling = 1E-8
  [../]
  [./disp_x]
    scaling = 1E-10
  [../]
  [./disp_y]
    scaling = 1E-10
  [../]
  [./disp_z]
    scaling = 1E-10
  [../]
[]
###########################################################
[AuxVariables]
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = -${sigmaH_ini_bc}
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = -${sigmaH_ini_bc}
  [../]
  [./stress_zz]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = -${sigmaV_ini_bc}
  [../]
  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./porosity]
    family = monomial
    order = constant
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
  [./stress_xx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xx
    index_i = 0
    index_j = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  [../]
  [./stress_zz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zz
    index_i = 2
    index_j = 2
  [../]
  [./density]
    type = MaterialRealAux
    variable = density
    property = PorousFlow_fluid_phase_density_qp0
    execute_on = TIMESTEP_END
  [../]
  [./viscosity]
    type = MaterialRealAux
    variable = viscosity
    property = PorousFlow_viscosity_qp0
    execute_on = TIMESTEP_END
  [../]
  [./porosity]
    type = MaterialRealAux
    property = PorousFlow_porosity_qp
    variable = porosity
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
[BCs]
  [./roller_y]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary =  south
  [../]
  [./total_stress_y]
    type = NeumannBC
    variable = disp_y
    value =  -${sigmaH_ini_bc}
    boundary = north
  [../]

  [./roller_x]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary =  west
  [../]
  [./total_stress_x]
    type = NeumannBC
    variable = disp_x
    value =  -${sigmaH_ini_bc}
    boundary = east
  [../]

  [./roller_z]
    type = DirichletBC
    variable = disp_z
    value = 0
    boundary = bottom
  [../]
  [./overburden_total_stress]
    type = NeumannBC
    variable = disp_z
    value =  -${sigmaV_ini_bc}
    boundary = top
  [../]
[]

############################################################
[DiracKernels]
  [./summer_injection_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 -9.99 -9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./summer_injection_T]
    type = FunctionEnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 -9.99 -9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    function = T_inj_function
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./summer_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 9.99 9.99 3.6'
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
    line_base = '1 9.99 9.99 3.6'
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
    line_base = '1 9.99 9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./winter_injection_T]
    type = FunctionEnthalpySink
    fluid_phase = 0
    variable = temperature
    SumQuantityUO = heat_enthalpy_in_inc
    line_base = '1 9.99 9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    pressure = porepressure
    function = T_inj_function
    fp = tabulated_water
    p_or_t_vals = '-1e9 1e9'
    fluxes = '-${inj_ext_flux} -${inj_ext_flux}'
  [../]
  [./winter_production_P]
    type = PorousFlowPolyLineSink
    fluid_phase = 0
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 -9.99 -9.99 3.6'
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
    line_base = '1 -9.99 -9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_mobility = false
    use_enthalpy = true
    p_or_t_vals = '-1e9 1e9'
    fluxes = '${inj_ext_flux} ${inj_ext_flux}'
  [../]

 [./rest_injection_P]
    type = PorousFlowPolyLineSink
    variable = porepressure
    SumQuantityUO = fluid_mass_in_inc
    line_base = '1 -9.99 -9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = 0
    fluxes = 0
  [../]
  [./rest_injection_T]
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
    p_or_t_vals = 0
    fluxes = 0
  [../]
  [./rest_production_P]
    type = PorousFlowPolyLineSink
    variable = porepressure
    SumQuantityUO = fluid_mass_out_inc
    line_base = '1 9.99 9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    p_or_t_vals = 0
    fluxes = 0
  [../]
  [./rest_production_T]
    type = PorousFlowPolyLineSink
    variable = temperature
    SumQuantityUO = heat_enthalpy_out_inc
    line_base = '1 9.99 9.99 3.6'
    line_length = ${well_length}
    line_direction = '0 0 1'
    use_enthalpy = true
    p_or_t_vals = 0
    fluxes = 0
  [../]
[]
############################################################
[Controls]
  [summer_injection]
    type = ConditionalFunctionEnableControl
    conditional_function = inj_function_summer
    enable_objects  = 'DiracKernels::summer_injection_P DiracKernels::summer_injection_T DiracKernels::summer_production_P DiracKernels::summer_production_T'
  []
  [winter_extraction]
    type = ConditionalFunctionEnableControl
    conditional_function = inj_function_winter
    enable_objects  = 'DiracKernels::winter_injection_P DiracKernels::winter_injection_T DiracKernels::winter_production_P DiracKernels::winter_production_T'
  []
  [rest]
    type = ConditionalFunctionEnableControl
    conditional_function = rest_function
    enable_objects  = 'DiracKernels::rest_injection_P  DiracKernels::rest_injection_T DiracKernels::rest_production_P  DiracKernels::rest_production_T'
  []
[]
[Functions]
  [./inj_function_summer]
    type = ParsedFunction
    value = ' ((t/24/3600/365.25-floor(t/24/3600/365.25))>0.375) & ((t/24/3600/365.25-floor(t/24/3600/365.25)) <=0.625) '
  [../]
  [./inj_function_winter]
    type = ParsedFunction
    value = '((t/24/3600/365.25-floor(t/24/3600/365.25))<=0.125) | ((t/24/3600/365.25-floor(t/24/3600/365.25))>0.875)'
  [../]
  [./T_inj_function]
    type = ParsedFunction
    value = '(453.15+399.15)/2+(453.15-399.15)/2*sin((-0.25+t/24/3600/365.25-floor(t/24/3600/365.25))*2*3.1415926535897932)'
  [../]
  [./rest_function]
    type = ParsedFunction
    value = '((t/24/3600/365.25-floor(t/24/3600/365.25))>0.125 & (t/24/3600/365.25-floor(t/24/3600/365.25)) <= 0.375) | ((t/24/3600/365.25-floor(t/24/3600/365.25))>0.625 & (t/24/3600/365.25-floor(t/24/3600/365.25)) <=0.875) '
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
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    shear_modulus = 8.38E9
    poissons_ratio = 0.225
  [../]
  [./strain]
    type = ComputeSmallStrain
    eigenstrain_names = 'thermal_contribution ini_stress'
  [../]
  [./ini_strain]
    type = ComputeEigenstrainFromInitialStress
    initial_stress = '-${sigmaH_ini_bc} 0 0  0 -${sigmaH_ini_bc} 0  0 0 -${sigmaV_ini_bc}' 
    eigenstrain_name = ini_stress
  [../]
  [./thermal_contribution]
    type = ComputeThermalExpansionEigenstrain
    temperature = temperature
    stress_free_temperature = ${T_ini_bc}   # using the intial temperature
    thermal_expansion_coeff = 1.0e-5  # this is the linear thermal expansion coefficient
    eigenstrain_name = thermal_contribution
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]

  [./density_aquifer]
    type = GenericConstantMaterial
    prop_names = density
    prop_values = 2600.0
  [../]

  [./internal_energy_aquifer]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 920.0 
    density = 2600.0
    block = 'inj_well ext_well caps layer_1 layer_2 layer_3 layer_4 layer_5 layer_6 layer_7 layer_8 layer_9 layer_10 layer_11 layer_12 layer_13 layer_14 layer_15 layer_16 layer_17 layer_18 layer_19 layer_20'
  [../]
  [./thermal_conductivity_aquifer]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.51 0 0  0 2.51 0  0 0 2.51'
    block = 'inj_well ext_well caps layer_1 layer_2 layer_3 layer_4 layer_5 layer_6 layer_7 layer_8 layer_9 layer_10 layer_11 layer_12 layer_13 layer_14 layer_15 layer_16 layer_17 layer_18 layer_19 layer_20'
  [../]
  [./porosity_well]
    type = PorousFlowPorosityConst
    block = 'inj_well ext_well'
    porosity = 0.299
  [../]
  [./permeability_well]
    type = PorousFlowPermeabilityConst
    block = 'inj_well ext_well'
    permeability = '.2660E-12 0 0   0 .2660E-12 0   0 0 .2330E-09'
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
    reference_porepressure = ${pp_ini_bc} # using the intial pore presure
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_1]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_1
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.3887'
    f = 9.9856e-12
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_2]
    type = PorousFlowPorosity
    block = layer_2
    porosity_zero = 0.158
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_2]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_2
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.3902'
    f = 2.4912e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_3]
    type = PorousFlowPorosity
    block = layer_3
    porosity_zero = 0.176
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_3]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_3
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.3920'
    f = 4.5031e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_4]
    type = PorousFlowPorosity
    block = layer_4
    porosity_zero = 0.188
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_4]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_4
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.7292'
    f = 5.3904e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_5]
    type = PorousFlowPorosity
    block = layer_5
    porosity_zero = 0.166
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_5]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_5
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.7091'
    f = 3.0350e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_6]
    type = PorousFlowPorosity
    block = layer_6
    porosity_zero = 0.185
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_6]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_6
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.6723'
    f = 5.4254e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_7]
    type = PorousFlowPorosity
    block = layer_7
    porosity_zero = 0.24
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_7]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_7
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9737'
    f = 6.6083e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_8]
    type = PorousFlowPorosity
    block = layer_8
    porosity_zero = 0.26
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_8]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_8
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9281'
    f = 6.4064e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_9]
    type = PorousFlowPorosity
    block = layer_9
    porosity_zero = 0.285
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_9]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_9
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9434'
    f = 5.7640e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_10]
    type = PorousFlowPorosity
    block = layer_10
    porosity_zero = 0.27
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_10]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_10
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9693'
    f = 6.0536e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_11]
    type = PorousFlowPorosity
    block = layer_11
    porosity_zero = 0.23
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_11]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_11
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.7779'
    f = 7.0930e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_12]
    type = PorousFlowPorosity
    block = layer_12
    porosity_zero = 0.306
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_12]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_12
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 1'
    f = 5.1702e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_13]
    type = PorousFlowPorosity
    block = layer_13
    porosity_zero = .285
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_13]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_13
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9577'
    f = 5.7912e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_14]
    type = PorousFlowPorosity
    block = layer_14
    porosity_zero = .155
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_14]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_14
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.0203'
    f = 8.6194e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_15]
    type = PorousFlowPorosity
    block = layer_15
    porosity_zero = .299
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_15]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_15
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.8759'
    f = 5.4697e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_16]
    type = PorousFlowPorosity
    block = layer_16
    porosity_zero = .334
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_16]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_16
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9716'
    f = 4.5139e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_17]
    type = PorousFlowPorosity
    block = layer_17
    porosity_zero = .32
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_17]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_17
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9270'
    f = 4.9059e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_18]
    type = PorousFlowPorosity
    block = layer_18
    porosity_zero = .294
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_18]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_18
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.9752'
    f = 5.4915e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_19]
    type = PorousFlowPorosity
    block = layer_19
    porosity_zero = .279
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_19]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_19
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.6390'
    f = 6.3038e-11
    d = 1
    m = 2
    n = 5
  [../]

  [./porosity_layer_20]
    type = PorousFlowPorosity
    block = layer_20
    porosity_zero = .071
    fluid = true
    biot_coefficient = 0.8
    reference_porepressure = ${pp_ini_bc}
    solid_bulk = 1.5E8
    mechanical = true
    thermal_expansion_coeff = 1.0e-5 # Kim and Hosseini 2013
    thermal  = true
    reference_temperature = ${T_ini_bc} # using the intial temperature
  [../]
  [./permeability_layer_20]
    type = PorousFlowPermeabilityKozenyCarman
    block = layer_20
    poroperm_function = kozeny_carman_fd2
    k_anisotropy = '1 0 0   0 1 0   0 0 0.0370'
    f = 1.2915e-11
    d = 1
    m = 2
    n = 5
  [../]
[]

############################################################
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
[Functions]
  [./Fiss_Function]
    type = PiecewiseLinear
    x ='0
    3944700     11834100    19723500    27612900
    35502300    43391700    51281100    59170500
    67059900    74949300    82838700    90728100
    98617500    106506900   114396300   122285700
    130175100   138064500   145953900   153843300
    161732700   169622100   177511500   185400900
    193290300   201179700   209069100   216958500
    224847900   232737300   240626700   248516100
    256405500   264294900   272184300   280073700
    287963100   295852500   303741900   311631300
    315576000'
    y = '0
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        1 2 3 4
        5'
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
  end_time = 315576000
  dtmax = 432000 # 5 days
  dtmin = 10
 [./TimeStepper]
   type = IterationAdaptiveDT
   dt = 1000.0
   optimal_iterations = 15
   force_step_every_function_point =  true
   timestep_limiting_function = Fiss_Function
 [../]
  line_search = none
# controls for linear iterations
  l_max_its = 50
  l_tol = 1e-4
# controls for nonlinear iterations
  nl_max_its = 50
  nl_abs_tol = 1e-6
[]
###########################################################
[Outputs]

[./exodus]
    type = Exodus
    elemental_as_nodal = true
    sync_only = true
    sync_times = '0
    3944700     11834100    19723500    27612900
    35502300    43391700    51281100    59170500
    67059900    74949300    82838700    90728100
    98617500    106506900   114396300   122285700
    130175100   138064500   145953900   153843300
    161732700   169622100   177511500   185400900
    193290300   201179700   209069100   216958500
    224847900   232737300   240626700   248516100
    256405500   264294900   272184300   280073700
    287963100   295852500   303741900   311631300
    315576000'
[../]

[./Console]
  type = Console
  #perf_log = false
  output_linear = false
  output_nonlinear = true
  interval = 1
[../]

[./CSV]
  type = CSV
  interval = 1
[../]
[]
###########################################################
