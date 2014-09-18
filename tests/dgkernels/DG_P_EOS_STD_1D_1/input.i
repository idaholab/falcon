#### Authors: Yidong Xia (Yidong.Xia@inl.gov)
#### Created: 09/16/2014

[Mesh]
  type =  GeneratedMesh
  dim  =  3
  nx   =  10
  xmax =  100.0
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  console = true
[]

[Executioner]
  type = Transient
  dt = 100.0
  num_steps = 5
  solve_type = PJFNK
  nl_abs_tol = 1e-8
  nl_rel_tol = 1e-6
[]

[Variables]
  [./pressure]
    order = FIRST
    family = L2_LAGRANGE
    initial_condition = 1e6 #[Pa]
  [../]
  [./temperature]
    order = FIRST
    family = L2_LAGRANGE
    initial_condition = 460 #[K]
  [../]
[]

[Kernels]
  #### Pressure time derivative kernel for PT (single-phase) problems
  [./p_td] 
    type = MassFluxTimeDerivative_PT
    variable = pressure
  [../]
  #### Mass diffusion kernel for PT (single-phase) problems
  [./p_wmfp] 
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_d] 
    type = TemperatureDiffusion
    variable = temperature
  [../]
[]

[DGKernels]
  #### Mass diffusion kernel for PT (single-phase) problems
  [./p_diff]
    type = DGMaterialDiffusion
    variable = pressure
    prop_name = tau_water
  [../]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_diff] 
    type = DGMaterialDiffusion
    variable = temperature
    prop_name = thermal_conductivity
  [../]
[]

[Functions]
  [./bc_p_left]
    type = ParsedFunction
    value = 2e6 #[Pa]
  [../]
  [./bc_p_right]
    type = ParsedFunction
    value = 1e6 #[Pa]
  [../]
  [./bc_t_left]
    type = ParsedFunction
    value = 460 #[K]
  [../]
  [./bc_t_right]
    type = ParsedFunction
    value = 460 #[K]
  [../]
[]

[BCs]
  [./left_p]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = left
    variable  = pressure
    prop_name = tau_water
    function  = bc_p_left
  [../]
  [./right_p]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = right
    variable  = pressure
    prop_name = tau_water
    function  = bc_p_right
  [../]
  [./left_t]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = left
    variable  = temperature
    prop_name = thermal_conductivity
    function  = bc_t_left
  [../]
  [./right_t]
    type      = DGFunctionMaterialDiffusionBC
    boundary = right
    variable = temperature
    prop_name = thermal_conductivity
    function = bc_t_right
  [../]
[]

[AuxVariables]
  [./gradP_x]
    order  = CONSTANT
    family = MONOMIAL
  [../]
  [./density_water]
    order  = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity_water]
    order  = CONSTANT
    family = MONOMIAL
  [../]
  [./tau_water]
    order  = CONSTANT
    family = MONOMIAL
  [../]
  [./darcy_v_x]
    order  = CONSTANT
    family = MONOMIAL
  [../]
  [./v_x]
    order  = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./gradP_x]
    type = VariableGradientAux
    variable = gradP_x
    coupled_variable = pressure
    component = 0
  [../]
  [./density_water]
    type = MaterialRealAux
    variable = density_water
    property = density_water
  [../]
  [./viscosity_water]
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
  [../]
  [./tau_water]
    type = MaterialRealAux
    variable = tau_water
    property = tau_water
  [../]
  [./darcy_v_x]
    type = DarcyFluxAux
    variable = darcy_v_x
    component = 0
  [../]
  [./v_x]
    type = VelocityAux
    variable = v_x
    component = 0
  [../]
[]

[Materials]

  [./GeothermalMaterial]
    block = 0

    #### flag booleans to define THMC problem
    heat_transport              = true      # T - flaged true to provide thermal_conductivity to temp diffusion kernel even though this is not a T problem
    fluid_flow                  = true      # H -
    solid_mechanics             = false     # M
    chemical_reactions          = false     # C

    #### couple in main NL variables
    pressure                    = pressure
    temperature                 = temperature

    #### material property inputs from PorousMedia (base class - parameters availible to all THMC materials)
    gravity                     = 0.0       # gravity magnitude [m/s^2]
    gx                          = 0.0       # x-component of gravity vector
    gy                          = 0.0       # y-component of gravity vector
    gz                          = 1.0       # z-component of gravity vector
    porosity                    = 0.2
    permeability                = 1.0e-12   # [m^2]
   #compressibility             = 1e-15

    #### material property inputs from HeatTransport
    specific_heat_water         = 4186      # [J/(kg.K)]
    specific_heat_rock          = 920       # [J/(kg.K)]
    thermal_conductivity        = 2.5       # [W/(kg.K)]

    #### material property inputs from FluidFlow (must have temp_dependent_fluid_props = true and temperature coupled in to get varaible density/viscosity
    temp_dependent_fluid_props  = true      # we want to have variable density and viscosity in this problem, so we use the water/steam EOS routine
    water_steam_properties      = water_steam_properties    # coupling of WaterSteamEOS UserObject below to use for calculation of fluid props

  [../]
[]

[UserObjects]
  #### This user object contains the water and steam equations of state that are used to calculate variable fluid props
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]
