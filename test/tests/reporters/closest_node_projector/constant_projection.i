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
    real_names = 'pt_value pt_x pt_y pt_z'
    real_values = '6 .7 .3 -.1'
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
  [out]
    type = JSON
    execute_system_information_on = none
  []
[]
