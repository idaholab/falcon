[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 1
  nz = 1
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 0.1
  zmin = 0
  zmax = 1
[]

[GlobalParams]
  PorousFlowDictator = dictator
  block = 0
[]

[UserObjects]
  [dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'porepressure'
    number_fluid_phases = 1
    number_fluid_components = 1
  []
  [pc]
    type = PorousFlowCapillaryPressureVG
    m = 0.8
    alpha = 1e-5
  []
[]

[Variables]
  [porepressure]
  []
[]

[AuxVariables]
  [./disp_x]
  #initial_condition=0
  [../]
  [./disp_y]
  #initial_condition=0
  [../]
  [./disp_z]
  #initial_condition=0
  [../]
  [porosity_out]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [porosity]
    type = MaterialRealAux
    variable = porosity_out
    property= PorousFlow_porosity_qp
  []
[]

[BCs]
  [xmax_drained]
    type = DirichletBC
    variable = porepressure
    value = 0
    boundary = right
  []
[]

[Kernels]
  [poro_vol_exp]
    type = PorousFlowMassVolumetricExpansion
    variable = porepressure
    fluid_component = 0
  []
  [mass0]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = porepressure
  []
  [flux]
    type = PorousFlowAdvectiveFlux
    variable = porepressure
    gravity = '0 0 0'
    fluid_component = 0
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    bulk_modulus = 8
    density0 = 1
    thermal_expansion = 0
    viscosity = 1
  []
[]

[Materials]
  [temperature]
    type = PorousFlowTemperature
  []
  [eff_fluid_pressure]
    type = PorousFlowEffectiveFluidPressure
  []
  [ppss]
    type = PorousFlow1PhaseP
    porepressure = porepressure
    capillary_pressure = pc
  []
  [massfrac]
    type = PorousFlowMassFraction
  []
  [simple_fluid]
    type = PorousFlowSingleComponentFluid
    fp = simple_fluid
    phase = 0
  []
  [total_strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  []
  [vol_strain]
    displacements = 'disp_x disp_y disp_z'
    type = PorousFlowVolumetricStrain
  []
  [porosity]
    type = PorousFlowPorosityHMBiotModulus
    porosity_zero = 0.1
    biot_coefficient = 0.6
    solid_bulk = 1
    constant_fluid_bulk_modulus = 8
    constant_biot_modulus = 4.7058823529
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1.5 0 0   0 1.5 0   0 0 1.5'
  []
  [relperm]
    type = PorousFlowRelativePermeabilityCorey
    n = 0 # unimportant in this fully-saturated situation
    phase = 0
  []
[]

[Postprocessors]
  [dt]
    type = FunctionValuePostprocessor
    outputs = console
    function = if(0.15*t<0.01,0.15*t,0.01)
  []
[]


[Preconditioning]
  [andy]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = 'gmres asm lu 1E-14 1E-10 10000'
  []
[]

[Executioner]
  type = Transient
  solve_type = Newton
  start_time = 0
  end_time = 0.7
  [TimeStepper]
    type = PostprocessorDT
    postprocessor = dt
    dt = 0.001
  []
[]
