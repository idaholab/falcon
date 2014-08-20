#### Author : Yidong Xia (Yidong.Xia@inl.gov)
#### Created: 08/19/2014
#### Description:
#### This test case is designed to validate and verify
#### the DGWaterMassFluxPressure_PT kernel.
#### The overall settings are similar to
#### /falcon/tests/examples/P_CONST_STD_1D_1.i

[Mesh]
  type =  GeneratedMesh
  dim  =  3
  nx   =  100
  xmax =  100.0
  ymax =   10.0
[]

[Outputs]
  file_base = DG_P_CONST_STD_1D_1_out
  output_initial = true
  exodus = true
  console = true
[]

[Executioner]
  type = Steady
[]

[Variables]
  ##### Main pressure variable declared
  [./pressure] 
    order = FIRST
    family = L2_LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = 0.1e6 # [Pa]
    [../]
  [../]
[]

[Kernels]
  #### Mass diffusion kernel for PT (single-phase) problems
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
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
[]

[Functions]
  [./bc_left]
    type = ParsedFunction
    value = 0.11e6
  [../]
  [./bc_right]
    type = ParsedFunction
    value = 0.10e6
  [../]
[]

[BCs]
  [./left_p]
    type = DGFunctionDiffusionDirichletBC
    variable = pressure
    boundary = left
    function = bc_left
    epsilon = 1.0
    sigma = 2.0
  [../]
  [./right_p]
    type = DGFunctionDiffusionDirichletBC
    variable = pressure
    boundary = right
    function = bc_right
    epsilon = 1.0
    sigma = 2.0
  [../]
[]

[Materials]

  [./GeothermalMaterial]
    block = 0

    #### flag booleans to define THMC problem
    heat_transport              = false     # T
    fluid_flow                  = true      # H
    solid_mechanics             = false     # M
    chemical_reactions          = false     # C

    #### couple in main NL variable
    pressure                    = pressure

    #### material property inputs from PorousMedia 
    #### (base class - parameters availible to all THMC materials)
    gravity                     = 0.0       # gravity magnitude [m/s^2]
    gx                          = 0.0       # x-component of gravity vector
    gy                          = 0.0       # y-component of gravity vector
    gz                          = 1.0       # z-component of gravity vector
    porosity                    = 0.5
    permeability                = 1.0e-12   # [m^2]

    #### material property inputs from FluidFlow
    temp_dependent_fluid_props  = false     # we want to have consant density and viscosity in this problem
    constant_density            = 1000      # [kg/m^2]
    constant_viscosity          = 0.12e-3   # [Pa.s]

  [../]
[]

[AuxVariables]
  ##### This aux variable is not necessary to run the problem
  ##### it just displays the velocity
  [./v_x] 
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  #### This aux kernel is not necessary to run the problem
  #### it just displays the velocity
  [./vx]
    type = VelocityAux
    variable = v_x
    component = 0
  [../]
[]
