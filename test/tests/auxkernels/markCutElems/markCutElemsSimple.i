#each element has length = 4/5
#each element volume is 64/125
#rectangle.e cuts into 9 elements
# volume of original mesh = 4x4x4=64
# volume of marked elements = (64*9/125)=4.608
# volume of unmarked element = 64-(64*9/125)=59.392
# rectangle.e cutter mesh block_id is 10
# uncut_value is -1
#pp ElementIntegralVariablePostprocessor outputs (64*9/125)*10 - 59.392 = -13.312
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
    uncut_value = -1
    use_block_ids = true
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
