# A confined aquifer is fully saturated with water
# Earth tides apply strain to the aquifer and the resulting porepressure changes are recorded
#
# To replicate standard poroelasticity exactly:
# (1) the PorousFlowBasicTHM Action is used;
# (2) multiply_by_density = false;
# (3) PorousFlowConstantBiotModulus is used
[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 1
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  block = 0
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
  [CGpressure]
  []
  [./DGpressure]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

[BCs]
  # zmin is called 'back'
  # zmax is called 'front'
  # ymin is called 'bottom'
  # ymax is called 'top'
  # xmin is called 'left'
  # xmax is called 'right'
  [strain_x]
    type = FunctionDirichletBC
    variable = disp_x
    function = earth_tide_x
    boundary = 'left right'
  []
  [strain_y]
    type = FunctionDirichletBC
    variable = disp_y
    function = earth_tide_y
    boundary = 'bottom top'
  []
  [strain_z]
    type = FunctionDirichletBC
    variable = disp_z
    function = earth_tide_z
    boundary = 'back front'
  []
[]

[AuxVariables]
  [EGpressure]
    family = MONOMIAL
    order = FIRST
  []
[]

[AuxKernels]
  [EGpressure]
    type = ParsedAux
    args = 'CGpressure DGpressure'
    variable = EGpressure
    function = 'CGpressure + DGpressure'
  []
[]

[Kernels]
  [./grad_stress_x]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./grad_stress_y]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
  [./grad_stress_z]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
  [../]
    [./poro_x]
    type = PoroMechanicsCoupling
    variable = disp_x
    component = 0
    porepressure = CGpressure
  [../]
  [./poro_y]
    type = PoroMechanicsCoupling
    variable = disp_y
    component = 1
    porepressure = CGpressure
  [../]
  [./poro_z]
    type = PoroMechanicsCoupling
    variable = disp_z
    component = 2
    porepressure = CGpressure
  [../]
  [./poro_xDG]
    type = PoroMechanicsCoupling
    variable = disp_x
    component = 0
    porepressure = DGpressure
  [../]
  [./poro_yDG]
    type = PoroMechanicsCoupling
    variable = disp_y
    component = 1
    porepressure = DGpressure
  [../]
  [./poro_zDG]
    type = PoroMechanicsCoupling
    variable = disp_z
    component = 2
    porepressure = DGpressure
  [../]
  [./poro_timederiv]
    type = PoroFullSatTimeDerivative
    variable = CGpressure
  [../]
  [./poro_timederiv2]
    type = PoroFullSatTimeDerivative
    variable = DGpressure
  [../]
  [./darcy_flow]
    type = DarcyFluxPressure
    variable = CGpressure
    gravity = '0 0 0'
  [../]
  [./time_derivEG1]
    type = EGPoroFullSatTimeDerivative
    variable = CGpressure
    v = DGpressure
  [../]
  [./time_derivEG2]
    type = EGPoroFullSatTimeDerivative
    variable = DGpressure
    v = CGpressure
  [../]
[]
[DGKernels]
  [./DG]
    type = DGDiffusion
    variable = DGpressure
    sigma = 10
    diff = conductivity
    epsilon = 0
  [../]
  [./EG1]
    type = EGDiffusion
    variable = DGpressure
    sigma = 10
    diff = conductivity
    epsilon = 0
    v = CGpressure
  [../]
  [./EG2]
    type = EGDiffusion
    variable = CGpressure
    v = DGpressure
    sigma = 10
    diff = conductivity
    epsilon = 0
  [../]
[]

[Functions]
  [earth_tide_x]
    type = ParsedFunction
    value = 'x*1E-8*(5*cos(t*2*pi) + 2*cos((t-0.5)*2*pi) + 1*cos((t+0.3)*0.5*pi))'
  []
  [earth_tide_y]
    type = ParsedFunction
    value = 'y*1E-8*(7*cos(t*2*pi) + 4*cos((t-0.3)*2*pi) + 7*cos((t+0.6)*0.5*pi))'
  []
  [earth_tide_z]
    type = ParsedFunction
    value = 'z*1E-8*(7*cos((t-0.5)*2*pi) + 4*cos((t-0.8)*2*pi) + 7*cos((t+0.1)*4*pi))'
  []
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    bulk_modulus = 10.0E9 # drained bulk modulus
    poissons_ratio = 0.25
  [../]
  [./strain]
    type = ComputeSmallStrain
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
  [./poro_material]
    type = PoroFullSatMaterial
    porosity0 = 0.1
    biot_coefficient = 0.6
    solid_bulk_compliance = 1E-10
    fluid_bulk_compliance = 5E-10
    constant_porosity = true
    porepressure = EGpressure
  [../]
  [density]
    type = GenericConstantMaterial
    prop_names = density
    prop_values = 1000.0
  []
  [conductivity]
    type = GenericConstantMaterial
    prop_names = conductivity
    prop_values = 1E-9
  []
[]

[Postprocessors]
  [pp]
    type = PointValue
    point = '0.5 0.5 0.5'
    variable = EGpressure
  []
[]

[Executioner]
  type = Transient
  solve_type = Newton
  dt = 0.01
  end_time = 2

  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
[]

[Outputs]
  file_base = EG_Tides
  console = true
  csv = true
[]
