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
  type = Steady
[]

[Variables]
  #### Main pressure variable declared
  [./pressure]
    order = FIRST
    family = L2_LAGRANGE
    initial_condition = 10e6 #[Pa]
  [../]
[]

[Kernels]
  #### Mass diffusion kernel for PT (single-phase) problems
  [./p_diff]
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
[]

[DGKernels]
  #### Material diffusion kernel for pressure diffusion
  [./p_diff]
    type      = DGMaterialDiffusion
    variable  = pressure
    prop_name = tau_water
  [../]
[]

[Functions]
  [./bc_left]
    type  = ParsedFunction
    value = 20e6 #[Pa]
  [../]
  [./bc_right]
    type  = ParsedFunction
    value = 10e6 #[Pa]
  [../]
[]

[BCs]
  [./left_p]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = left
    variable  = pressure
    prop_name = tau_water
    function  = bc_left
  [../]
  [./right_p]
    type      = DGFunctionMaterialDiffusionBC
    boundary  = right
    variable  = pressure
    prop_name = tau_water
    function  = bc_right
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

    # flag booleans to define THMC problem
    heat_transport              = false     # T
    fluid_flow                  = true      # H
    solid_mechanics             = false     # M
    chemical_reactions          = false     # C

    # couple in main NL variable
    pressure                    = pressure

    # material property inputs from PorousMedia (base class - parameters availible to all THMC materials)
    gravity                     = 0.0       # gravity magnitude [m/s^2]
    gx                          = 0.0       # x-component of gravity vector
    gy                          = 0.0       # y-component of gravity vector
    gz                          = 1.0       # z-component of gravity vector
    porosity                    = 0.5
    permeability                = 1.0e-12   # [m^2]

    # material property inputs from FluidFlow
    temp_dependent_fluid_props  = false     # we want to have consant density and viscosity in this problem
    constant_density            = 1000      # [kg/m^2]
    constant_viscosity          = 0.12e-3   # [Pa.s]

  [../]
[]
