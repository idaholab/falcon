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
  [coords]
    type=ClosestNodeProjector
    xcoord_name = x
    ycoord_name = y
    zcoord_name = z
    point = '.7 .3 -.1'
    value = '6'
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
