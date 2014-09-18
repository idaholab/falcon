#### Authors: Yidong Xia (Yidong.Xia@inl.gov)
#### Created: 09/16/2014

[Mesh]
  type = GeneratedMesh
  dim  = 3
  nx   = 10
  xmax = 100.0
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  console = true
[]

[Executioner]
  type = Steady
[]

[Variables]
  #### Main temperature variable declared
  [./temperature]
    order = FIRST
    family = L2_LAGRANGE
    initial_condition = 460 #[K]
  [../]
[]

[Kernels]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_d] 
    type = TemperatureDiffusion
    variable = temperature
  [../]
[]

[DGKernels]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_diff] 
    type = DGMaterialDiffusion
    variable = temperature
    prop_name = thermal_conductivity
  [../]
[]

[Functions]
  [./fn_t_left]
    type = ParsedFunction
    value = 370
  [../]
  [./fn_t_right]
    type = ParsedFunction
    value = 460
  [../]
[]

[BCs]
  [./left_t]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = left
    variable  = temperature
    prop_name = thermal_conductivity
    function  = fn_t_left
  [../]
  [./right_t]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = right
    variable  = temperature
    prop_name = thermal_conductivity
    function  = fn_t_right
  [../]
[]

[Materials]
  [./GeothermalMaterial]
    block = 0

    # flag booleans to define THMC problem
    heat_transport              = true      # T
    fluid_flow                  = true      # H - flaged true to provide fluid props even though this is not an H problem
    solid_mechanics             = false     # M
    chemical_reactions          = false     # C

    # couple in main NL variable
    temperature                 = temperature

    # material property inputs from PorousMedia (base class - parameters availible to all THMC materials)
    gravity                     = 0.0       # gravity magnitude [m/s^2]
    gx                          = 0.0       # x-component of gravity vector
    gy                          = 0.0       # y-component of gravity vector
    gz                          = 1.0       # z-component of gravity vector
    porosity                    = 0.2
    permeability                = 1.0e-13   # [m^2]

    # material property inputs from HeatTransport
    specific_heat_water         = 4186      # [J/(kg.K)]
    specific_heat_rock          = 920       # [J/(kg.K)]
    thermal_conductivity        = 2.5       # [W/(kg.K)]

    # material property inputs from FluidFlow
    temp_dependent_fluid_props  = false     # we want to have consant density and viscosity in this problem
    constant_density            = 1000      # [kg/m^2]
    constant_viscosity          = 0.12e-3   # [Pa.s]

  [../]
[]

[AuxVariables]
  #### This aux variables are not necessary to run the problem, it just display the viscosity
  [./viscosity_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  #### This aux variables are not necessary to run the problem, it just display the density
  [./density_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  #### This aux kernels are not necessary to run the problem, they just display the density
  [./density_water]
    type = MaterialRealAux
    variable = density_water
    property = density_water
  [../]
  #### This aux kernels are not necessary to run the problem, they just display the viscosity
  [./viscosity_water]
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
  [../]
[]
