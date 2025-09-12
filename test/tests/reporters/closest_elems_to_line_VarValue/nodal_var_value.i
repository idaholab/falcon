[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  xmin = 0
  xmax = 4
  ymin = 0
  ymax = 4
  elem_type = QUAD9
  # This test can only be run with renumering disabled, so the
  # NodalVariableValue postprocessor's node id is well-defined.
  allow_renumbering = false[]

  [Problem]
    solve = false
  []

[AuxVariables]
  [./v]
    order = SECOND
    family = LAGRANGE
  [../]

  [./v2]
    order = CONSTANT
    family = MONOMIAL
  [../]

[]

[AuxKernels]
  [v]
    type = ParsedAux
    use_xyzt = true
    variable = v
    function = 't*x*y'
  []
  [v2]
    type = ParsedAux
    use_xyzt = true
    variable = v2
    function = '-t*x*y'
  []
[]

[Reporters]
  [pt]
    type = ConstantReporter
    real_vector_names = 'pt1_x pt1_y pt1_z pt2_x pt2_y pt2_z'
    real_vector_values = '-1; -1; 0; 1; 1; 1'
    outputs = none
  []

  [no_var]
    type = ClosestElemsToLineVarValue
    projection_tolerance = 5
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    variable_to_sample = v
    outputs = out
  []
  [domain_var]
    type = ClosestElemsToLineVarValue
    projection_tolerance = 5
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    variable_filter = v
    variable_to_sample = v
    outputs = out
  []
  [elem_var]
    type = ClosestElemsToLineVarValue
    projection_tolerance = 5
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    variable_filter = v
    variable_to_sample = v2
    outputs = out
  []
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  dt = 1
  start_time = 0
  end_time = 3
[]

[Outputs]
  [out]
    type = JSON
    execute_system_information_on = none
      sync_times = '1 2 3'
      sync_only = true
  []
[]
