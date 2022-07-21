[Mesh]
  [cluster34]
    type = FileMeshGenerator
    file = 'Cluster_34_zid.e'
    use_for_exodus_restart = true
  []
[]

[Problem]
  solve = false
[]

[AuxVariables]
  [z_id]
    family = MONOMIAL
    order = CONSTANT
    initial_from_file_var = z_id
    initial_from_file_timestep = 'LATEST'
  []
[]

[Executioner]
  type = Steady
[]

[Reporters]
  [pt]
    type = ConstantReporter
    real_vector_names = 'pt1_x pt1_y pt1_z pt2_x pt2_y pt2_z'
    real_vector_values = '132.1; 97.5; 189.5; 136.5; 103.2; 50.6'
    outputs = none
  []

  [no_var]
    block = "9 10"
    type = ClosestElemsToLineWithValues
    value = 33
    projection_tolerance = 5
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    outputs = out
  []
  [domain_var]
    type = ClosestElemsToLineWithValues
    value = 22
    projection_tolerance = 5
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    variable = z_id
    outputs = out
  []
[]

[Outputs]
  [out]
    type = JSON
    execute_system_information_on = none
  []
[]
