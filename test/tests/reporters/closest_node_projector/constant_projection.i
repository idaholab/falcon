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
    points = '.3 .3 0
    .41 .31 0
    .49 .31 .2
    .55 .29 .1
    .62 .29 .1
    .7 .3 -.1'

    values = '1 2 3 4 5 6'
    projection_tolerance = 1
  []
[]

[Outputs]
  csv =true
[]
