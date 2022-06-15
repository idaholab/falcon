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

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[BCs]
  [./roller_xmin]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'left'
  [../]
  [./roller_ymin]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'bottom'
  [../]
  [./plane_strain]
    type = DirichletBC
    variable = disp_z
    value = 0
    boundary = 'back front'
  [../]
  [./top_velocity]
    type = FunctionDirichletBC
    variable = disp_y
    function = top_velocity
    boundary = top
  [../]
[]

[Functions]
  [./top_velocity]
    type = PiecewiseLinear
    x = '0 0.002 0.006   0.014   0.03    0.046   0.062   0.078   0.094   0.11    0.126   0.142   0.158   0.174   0.19 0.206 0.222 0.238 0.254 0.27 0.286 0.302 0.318 0.334 0.35 0.366 0.382 0.398 0.414 0.43 0.446 0.462 0.478 0.494 0.51 0.526 0.542 0.558 0.574 0.59 0.606 0.622 0.638 0.654 0.67 0.686 0.702'
    y = '-0.041824842    -0.042730269    -0.043412712    -0.04428867     -0.045509181    -0.04645965     -0.047268246 -0.047974749      -0.048597109     -0.0491467  -0.049632388     -0.050061697      -0.050441198     -0.050776675     -0.051073238      -0.0513354 -0.051567152      -0.051772022     -0.051953128 -0.052113227 -0.052254754 -0.052379865 -0.052490464 -0.052588233 -0.052674662 -0.052751065 -0.052818606 -0.052878312 -0.052931093 -0.052977751 -0.053018997 -0.053055459 -0.053087691 -0.053116185 -0.053141373 -0.05316364 -0.053183324 -0.053200724 -0.053216106 -0.053229704 -0.053241725 -0.053252351 -0.053261745 -0.053270049 -0.053277389 -0.053283879 -0.053289615'
  [../]
[]


[Kernels]
  [./grad_stress_x]
    type = StressDivergenceTensors
    variable = disp_x
    displacements = 'disp_x disp_y disp_z'
    component = 0
  [../]
  [./grad_stress_y]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    variable = disp_y
    component = 1
  [../]
  [./grad_stress_z]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    variable = disp_z
    component = 2
  [../]
  [poro_x]
    type = PorousFlowEffectiveStressCoupling
    PorousFlowDictator = dictator
    biot_coefficient = 0.6
    variable = disp_x
    component = 0
  []
  [poro_y]
    type = PorousFlowEffectiveStressCoupling
    PorousFlowDictator = dictator
    biot_coefficient = 0.6
    variable = disp_y
    component = 1
  []
  [poro_z]
    type = PorousFlowEffectiveStressCoupling
    PorousFlowDictator = dictator
    biot_coefficient = 0.6
    component = 2
    variable = disp_z
  []
[]

[AuxVariables]
  [./porepressure]
  initial_condition=0
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tot_force]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]


[AuxKernels]
  [./stress_yy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  [../]
  [./tot_force]
    type = ParsedAux
    args = 'stress_yy porepressure'
    execute_on = timestep_end
    variable = tot_force
    function = '-stress_yy+0.6*porepressure'
  [../]
[]

[UserObjects]
  [dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'disp_x disp_y disp_z'
    number_fluid_phases = 1
    number_fluid_components = 1
  []
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '0.5 0.75'
    # bulk modulus is lambda + 2*mu/3 = 0.5 + 2*0.75/3 = 1
    fill_method = symmetric_isotropic
  [../]
  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]

  [vol_strain]
    type = PorousFlowEffectiveFluidPressure
    PorousFlowDictator = dictator
  []
  [ppss]
    PorousFlowDictator = dictator
    type = PorousFlow1PhaseFullySaturated
    porepressure = porepressure
  []
[]

[Executioner]
  type = Transient
  solve_type = Newton
  start_time = 0
  end_time = 0.7

  petsc_options = '-ksp_snes_ew'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'

  #fixed_point_algorithm = 'secant'

  accept_on_max_picard_iteration = true
  picard_max_its = 2
  picard_rel_tol = 1e-20

  relaxation_factor = 0.5
  relaxed_variables = 'disp_x disp_y disp_z'

  [TimeStepper]
    type = PostprocessorDT
    postprocessor = dt
    dt = 0.001
  []
[]

[Postprocessors]
  [./p0]
    type = PointValue
    outputs = csv
    point = '0.0 0 0'
    variable = porepressure
  [../]
  [./p1]
    type = PointValue
    outputs = csv
    point = '0.1 0 0'
    variable = porepressure
  [../]
  [./p2]
    type = PointValue
    outputs = csv
    point = '0.2 0 0'
    variable = porepressure
  [../]
  [./p3]
    type = PointValue
    outputs = csv
    point = '0.3 0 0'
    variable = porepressure
  [../]
  [./p4]
    type = PointValue
    outputs = csv
    point = '0.4 0 0'
    variable = porepressure
  [../]
  [./p5]
    type = PointValue
    outputs = csv
    point = '0.5 0 0'
    variable = porepressure
  [../]
  [./p6]
    type = PointValue
    outputs = csv
    point = '0.6 0 0'
    variable = porepressure
  [../]
  [./p7]
    type = PointValue
    outputs = csv
    point = '0.7 0 0'
    variable = porepressure
  [../]
  [./p8]
    type = PointValue
    outputs = csv
    point = '0.8 0 0'
    variable = porepressure
  [../]
  [./p9]
    type = PointValue
    outputs = csv
    point = '0.9 0 0'
    variable = porepressure
  [../]
  [./p99]
    type = PointValue
    outputs = csv
    point = '1 0 0'
    variable = porepressure
  [../]
  [./xdisp]
    type = PointValue
    outputs = csv
    point = '1 0.1 0'
    variable = disp_x
  [../]
  [./ydisp]
    type = PointValue
    outputs = csv
    point = '1 0.1 0'
    variable = disp_y
  [../]
  [./total_downwards_force]
     type = ElementAverageValue
     outputs = csv
     variable = tot_force
  [../]

  [./dt]
    type = FunctionValuePostprocessor
    outputs = console
    function = if(0.15*t<0.01,0.15*t,0.01)
  [../]
[]

[Outputs]
  perf_graph = false
  exodus = false
  execute_on = 'timestep_end'
  file_base = mandel_master
  [csv]
    interval = 3
    type = CSV
  []
[]

[MultiApps]
  [./sub]
    type = TransientMultiApp
    app_type =FalconApp
    execute_on = TIMESTEP_END
    input_files = mandel_sub.i
  [../]
[]

[Transfers]
  [./disp_x_to_sub]
    type = MultiAppCopyTransfer
    direction = to_multiapp
    multi_app = sub
    source_variable = disp_x
    variable = disp_x
  [../]
  [./disp_y_to_sub]
    type = MultiAppCopyTransfer
    direction = to_multiapp
    multi_app = sub
    source_variable = disp_y
    variable = disp_y
  [../]
  [./disp_z_to_sub]
    type = MultiAppCopyTransfer
    direction = to_multiapp
    multi_app = sub
    source_variable = disp_z
    variable = disp_z
  [../]
  [./porepressure_from_sub]
    type = MultiAppCopyTransfer
    direction = from_multiapp
    multi_app = sub
    source_variable = porepressure
    variable = porepressure
  [../]
[]
