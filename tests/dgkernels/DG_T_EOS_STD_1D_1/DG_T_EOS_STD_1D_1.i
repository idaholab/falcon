#### Author : Yidong Xia (Yidong.Xia@inl.gov)
#### Created: 08/22/2014
#### Description:
#### This test case is designed to validate and verify
#### the DGTemperatureConvection kernel
#### and the DGFunctionConvection kernel.
#### But in fact the velocity is almost zero,
#### which is hard to assess the  convection.
#### The overall settings are similar to
#### /falcon/tests/examples/T_EOS_STD_1D_1.i
#### Special note:
#### the solution is quite sensitive to the choice of
#### the parameter sigma in the Interior Penalty method 

[Mesh]
  type = GeneratedMesh
  dim  = 3
  nx   = 100
  xmax = 10.0
[]

[Outputs]
  file_base = DG_T_EOS_STD_1D_1_out
  output_initial = true
  interval = 1 # default 10
  exodus = true
  console = true
[]

[Executioner]
  type = Transient
  num_steps = 2 # default 20
  dt = 10000.0
  #### Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  nl_abs_tol = 1e-6
[]

[Variables]
  #### Main pressure and temperature variables declared
  #### This is an constant pressure problem (heat transfer only), 
  #### but pressure is needed for calculation of fluid props
  #### which are both temp and pressure dependent
  [./pressure]
    order = FIRST
    family = L2_LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = 10.0e6 # [Pa]
    [../]
  [../]
  [./temperature]
    order = FIRST
    family = L2_LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = 473.15 # [K]
    [../]
  [../]
[]

[Kernels]
  #### Mass diffusion kernel for PT (single-phase) problems
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  #### Heat time derivative kernel for PT (single-phase) problems
  [./t_td]
    type = TemperatureTimeDerivative
    variable = temperature
  [../]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
  [../]
  #### Heat convection kernel for PT (single-phase) problems
  [./t_c]
    type = TemperatureConvection
    variable = temperature
  [../]
[]

[DGKernels]
  #### Mass diffusion kernel for PT (single-phase) problems
  [./p_dgwmfp]
    type = DGWaterMassFluxPressure_PT
    variable = pressure
    epsilon = 1.0
    sigma = 2.0
  [../]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_dgd]
    type = DGTemperatureDiffusion
    variable = temperature
    epsilon = 1.0
    sigma = 20.0
  [../]
  #### Heat convection kernel for PT (single-phase) problems
  [./t_dgc]
    type = DGTemperatureConvection
    variable = temperature
  [../]
[]

[Functions]
  [./p_leftBC]
    type = ParsedFunction
    value = 10e6
  [../]
  [./p_rightBC]
    type = ParsedFunction
    value = 10e6
  [../]
  [./t_leftBC]
    type = ParsedFunction
    value = 373.15
  [../]
  [./t_rightBC]
    type = ParsedFunction
    value = 473.15
  [../]
[]

[BCs]
  [./p_leftDiffusionBC]
    type = DGFunctionDiffusionDirichletBC
    variable = pressure
    boundary = left
    function = p_leftBC
    epsilon = 1.0
    sigma = 2.0
  [../]
  [./p_rightDiffusionBC]
    type = DGFunctionDiffusionDirichletBC
    variable = pressure
    boundary = right
    function = p_rightBC
    epsilon = 1.0
    sigma = 2.0
  [../]
  [./t_leftDiffusionBC]
    type = DGFunctionDiffusionDirichletBC
    variable = temperature
    boundary = left
    function = t_leftBC
    epsilon = 1.0
    sigma = 20.0
  [../]
  [./t_rightDiffusionBC]
    type = DGFunctionDiffusionDirichletBC
    variable = temperature
    boundary = right
    function = t_rightBC
    epsilon = 1.0
    sigma = 20.0
  [../]
  [./t_leftConvectionBC]
    type = DGFunctionConvectionBC
    variable = temperature
    boundary = left
    function = t_leftBC
  [../]
  [./t_rightConvectionBC]
    type = DGFunctionConvectionBC
    variable = temperature
    boundary = right
    function = t_rightBC
  [../]
[]

[Materials]
  [./GeothermalMaterial]
    block = 0

    #### flag booleans to define THMC problem
    heat_transport              = true      # T
    fluid_flow                  = true      # H - flaged true to provide fluid props to pressure diffusion kernel even though this is not a H problem
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
    permeability                = 1.0e-13   # [m^2]

    #### material property inputs from HeatTransport
    specific_heat_water         = 4186      # [J/(kg.K)]
    specific_heat_rock          = 920       # [J/(kg.K)]
    thermal_conductivity        = 2.5       # [W/(kg.K)]

    #### material property inputs from FluidFlow (must have temp_dependent_fluid_props = true and temperature coupled in to get varaible density/viscosity)
    temp_dependent_fluid_props  = true      # we want to have variable density and viscosity in this problem, so we use the water/steam EOS routine
    water_steam_properties      = water_steam_properties    # coupling of WaterSteamEOS UserObject below to use for calculation of fluid props

  [../]
[]

[UserObjects]
  #### This user object contains the water and steam equations of state 
  #### that are used to calculate variable fluid props
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[AuxVariables]
  #### This aux variables are not necessary to run the problem, it just display the velocity
  [./v_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  #### This aux variables are not necessary to run the problem
  #### it just display the viscosity
  [./viscosity_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  #### This aux variables are not necessary to run the problem
  #### it just display the density
  [./density_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  #### Thess aux kernels are not necessary to run the problem, 
  #### they just display the velocity/density/viscosity, 
  #### which change due to temperature effects

  #### This aux variables are not necessary to run the problem
  #### it just display the velocity
  [./vx]
    type = VelocityAux
    variable = v_x
    component = 0
  [../]
  #### This aux variables are not necessary to run the problem
  #### it just display the viscosity
  [./density_water]
    type = MaterialRealAux
    variable = density_water
    property = density_water
  [../]
  #### This aux variables are not necessary to run the problem
  #### it just display the density
  [./viscosity_water]
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
  [../]
[]
