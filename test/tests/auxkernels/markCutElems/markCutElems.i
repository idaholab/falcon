[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 10
    ny = 10
    nz = 10
    xmin = 0
    xmax = 200.0
    ymin = 0.0
    ymax = 200.0
    zmin = 0.0
    zmax = 200.0
    elem_type = HEX8
  []
[]

[Problem]
  solve = False
[]

[AuxVariables]
  [c]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Adaptivity]
  initial_steps = 4
  steps = 4
  marker = marker
  initial_marker = marker
  max_h_level = 4
  [Markers]
    [marker]
      type = ValueThresholdMarker
      variable = c
      coarsen = 0.09
      refine = 0.11
    []
  []
[]

[Executioner]
  type = Transient #Steady
  solve_type = PJFNK
  num_steps = 2
[]
[Postprocessors]
  [volume]
    type = ElementIntegralVariablePostprocessor
    variable = c
  []
[]

[Outputs]
  csv=true
[]
