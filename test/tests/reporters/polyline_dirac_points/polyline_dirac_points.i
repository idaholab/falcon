# Exercises PolylineDiracPoints on a fully 3D Cartesian mesh: 'bent_path' is a 3-waypoint,
# 2-segment L-bend (proving multi-segment/junction handling and 3D generality at once), and
# 'straight_path' is the simple 2-waypoint baseline case. The mesh is a 4x4x4 grid of unit cubes
# (0 to 4 in each direction), chosen so every generated point coordinate is a round number and can
# be checked by eye against the JSON output.
#
# 'bent_path': (0.5,0.5,0.5) -> (3.5,0.5,0.5) -> (3.5,0.5,3.5). Each segment crosses 4 elements
# (4 points each), but the two segments share their junction point (3.5,0.5,0.5) exactly, so the
# de-duplication logic must emit it only once: 4 + 4 - 1 = 7 points total, not 8.
#
# 'straight_path': (0.5,0.5,0.5) -> (2.5,0.5,0.5), crossing 3 elements -> 3 points.

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 4
    ny = 4
    nz = 4
    xmax = 4
    ymax = 4
    zmax = 4
  []
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Steady
[]

[Reporters]
  [bent_path]
    type = PolylineDiracPoints
    points = '0.5 0.5 0.5
              3.5 0.5 0.5
              3.5 0.5 3.5'
    weight = 0.1
    outputs = out
  []
  [straight_path]
    type = PolylineDiracPoints
    points = '0.5 0.5 0.5
              2.5 0.5 0.5'
    weight = 0.2
    outputs = out
  []
[]

[Outputs]
  [out]
    type = JSON
    execute_system_information_on = none
  []
[]
