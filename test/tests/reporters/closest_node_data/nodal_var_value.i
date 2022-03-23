[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  elem_type = QUAD9
  # This test can only be run with renumering disabled, so the
  # NodalVariableValue postprocessor's node id is well-defined.
  allow_renumbering = false[]

[AuxVariables]
  [./v]
    order = SECOND
    family = LAGRANGE
  [../]
[]

[AuxKernels]
  [v]
    type = ParsedAux
    use_xyzt = true
    variable = v
    function = 't*x*y'
  []
[]

[Variables]
  [./u]
    order = SECOND
    family = LAGRANGE
  [../]
[]

[Functions]
  [./force_fn]
    type = ParsedFunction
    value = '1-x*x+2*t'
  [../]

  [./exact_fn]
    type = ParsedFunction
    value = '(1-x*x)*t'
  [../]

  [./left_bc]
    type = ParsedFunction
    value = t
  [../]
[]

[Kernels]
  [./time_u]
    type = TimeDerivative
    variable = u
  [../]
  [./diff_u]
    type = Diffusion
    variable = u
  [../]
  [./ffn_u]
    type = BodyForce
    variable = u
    function = force_fn
  [../]
[]

[BCs]
  [./all_u]
    type = FunctionDirichletBC
    variable = u
    boundary = '1'
    function = exact_fn
  [../]
[]

[Postprocessors]
  #these are to check the reporter closestNodeData output
  inactive = 'node18u node18v node12u node12v'
  [./node18u]
    type = NodalVariableValue
    variable = u
    nodeid = 18
  [../]
  [./node18v]
    type = NodalVariableValue
    variable = v
    nodeid = 18
  [../]
  [./node12u]
    type = NodalVariableValue
    variable = u
    nodeid = 12
  [../]
  [./node12v]
    type = NodalVariableValue
    variable = v
    nodeid = 12
  [../]
[]

[Reporters]
  [pt12]
    type=ConstantReporter
    real_vector_names = 'pt_x pt_y pt_z'
    real_vector_values = '0.5; 0.5; 0'
  []
  [pt12p]
    type=ConstantReporter
    real_vector_names = 'pt_x pt_y pt_z'
    real_vector_values = '0.55; 0.45; 0.05'
  []
  [pt18]
    type=ConstantReporter
    real_vector_names = 'pt_x pt_y pt_z'
    real_vector_values = '0.75; 0.75; 0'
  []
  [pt18p]
    type=ConstantReporter
    real_vector_names = 'pt_x pt_y pt_z'
    real_vector_values = '0.76; 0.76; 0.3'
  []

  [node12]
    type=ClosestNodeData
    variable=v
    point_x = pt12/pt_x
    point_y = pt12/pt_y
    point_z = pt12/pt_z
    projection_tolerance = 0.1
    outputs = none
  []
  [node12p]
    type=ClosestNodeData
    variable=u
    point_x = pt12p/pt_x
    point_y = pt12p/pt_y
    point_z = pt12p/pt_z
    projection_tolerance = 0.1
    outputs = none
  []
  [node18]
    type=ClosestNodeData
    variable=v
    point_x = pt18/pt_x
    point_y = pt18/pt_y
    point_z = pt18/pt_z
    projection_tolerance = 0.1
    outputs = none
  []
  [node18p]
    type=ClosestNodeData
    variable=u
    point_x = pt18p/pt_x
    point_y = pt18p/pt_y
    point_z = pt18p/pt_z
    projection_tolerance = 1.0
    outputs = none
  []
  [accumulate]
    type = AccumulateReporter
    reporters = 'node12p/node_id node12p/node_x node12p/node_y node12p/node_z node12p/u node12/v
                 node18p/node_id node18p/node_x node18p/node_y node18p/node_z node18p/u node18/v'
    outputs = out
  []
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  dt = 0.1
  start_time = 0
  end_time = 1
[]

[Outputs]
  # execute_on = 'timestep_end'
  # csv = true
  # exodus = true
  [out]
    type = JSON
    execute_system_information_on = none
    execute_on = 'FINAL'
  []
[]
