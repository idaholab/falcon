#each element has length = 4/5
#each element volume is 64/125
#rectangle.e cuts into 9 elements
#pp ElementIntegralVariablePostprocessor outputs 64*9/125=4.608
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 5
    ny = 5
    nz = 5
    xmin = -2.01
    xmax = 1.99
    ymin = -2.01
    ymax = 1.99
    zmin = -2.01
    zmax = 1.99
    elem_type = HEX8
  []
[]

[AuxVariables]
  [c]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [ls]
    type = MarkCutElems
    mesh_file = rectangle.e
    variable = c
  []
[]

[Problem]
  solve = False
[]

[Executioner]
  type = Transient #Steady
  solve_type = PJFNK

  num_steps = 1
[]

[Postprocessors]
  [volume]
    type = ElementIntegralVariablePostprocessor
    variable = c
  []
[]

[Outputs]
  csv = true
[]
