[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  elem_type = QUAD4
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Steady
[]

[Reporters]
  [pt]
    type=ConstantReporter
    real_vector_names = 'pt_value pt_x pt_y pt_z'
    real_vector_values = '1 2 3 4 5 6;
                          .3 .41 .49 .55 .62 .7;
                          .3 .31 .31 .29 .29 .3;
                          0 0 0.2 0.1 0.1 -0.1'
    outputs = none
  []

  [node]
    type=ClosestNode
    point_x = pt/pt_x
    point_y = pt/pt_y
    point_z = pt/pt_z
    projection_tolerance = 1
    outputs = out
  []

  [coords]
    type=ClosestNodeProjector
    point_value =  pt/pt_value
    point_x = pt/pt_x
    point_y = pt/pt_y
    point_z = pt/pt_z
    projection_tolerance = 1
    outputs = out
  []
[]

[Outputs]
  csv = true
  [out]
    type = JSON
    execute_system_information_on = none
  []
[]
