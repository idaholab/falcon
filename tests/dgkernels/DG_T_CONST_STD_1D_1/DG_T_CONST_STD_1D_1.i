#### Author : Yidong Xia (Yidong.Xia@inl.gov)
#### Created: 08/19/2014
#### Description:
#### This test case is designed to validate and verify
#### the DGTemperatureDiffusion kernel.
#### The overall settings are similar to
#### /falcon/tests/examples/T_CONST_STD_1D_1.i

[Mesh]
  type = GeneratedMesh
  dim  = 3
  nx   = 100
  xmax = 10.0
[]

[Outputs]
  file_base = DG_T_CONST_STD_1D_1_out
  output_initial = true
  interval = 10
  exodus = true
  console = true
[]

[Executioner]
  type = Transient
  num_steps = 50
  dt = 10000.0

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  nl_abs_tol = 1e-6
[]

[Variables]
  [./temperature] # Main temperature variable declared
    order = FIRST
    family = L2_LAGRANGE # L2_LAGRANGE | MONOMIAL
    [./InitialCondition]
      type = ConstantIC
      value = 473.15 # [K]
    [../]
  [../]
[]

[Kernels]
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
[]

[DGKernels]
  #### Heat diffusion kernel for PT (single-phase) problems
  [./t_dgd] 
    type = DGTemperatureDiffusion
    variable = temperature
    epsilon = 1.0
    sigma = 2.0
  [../]
[]

[Functions]
  [./bc_left]
    type = ParsedFunction
    value = 373.15
  [../]
  [./bc_right]
    type = ParsedFunction
    value = 473.15
  [../]
[]

[BCs]
  [./left_t]
    type = DGFunctionDiffusionDirichletBC
    variable = temperature
    boundary = left
    function = bc_left
    epsilon = 1.0
    sigma = 2.0
  [../]
  [./right_t]
    type = DGFunctionDiffusionDirichletBC
    variable = temperature
    boundary = right
    function = bc_right
    epsilon = 1.0
    sigma = 2.0
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
