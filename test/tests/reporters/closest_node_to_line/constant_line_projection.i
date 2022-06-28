#The line segment defined in the constant reporter intersects exactly at the
#element centroid at the correct location with this mesh density.

[Mesh]
  type = MeshGeneratorMesh
  [dfn1]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 5
    ny = 5
    elem_type = QUAD4
  []
  [dfn2]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 5
    ny = 5
    elem_type = QUAD4
  []
  [dfn2_shift]
    type = TransformGenerator
    input = dfn2
    transform = TRANSLATE
    vector_value = '0 0 -4'
  []
  [dfn3]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 5
    ny = 5
    elem_type = QUAD4
  []
  [dfn3_shift]
    type = TransformGenerator
    input = dfn3
    transform = TRANSLATE
    vector_value = '0 0 2'
  []
  [dfns]
    type = MeshCollectionGenerator
    inputs = 'dfn1 dfn2_shift dfn3_shift'
  [../]
[]

[Problem]
  solve = false
[]

[AuxVariables]
  [oneID]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 3
  []
  [differentID]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 1
  []
[]

[AuxKernels]
  [oneID]
    type = ConstantAux
    variable = oneID
    value = 5
  []
  [differentID]
    type = ParsedAux
    variable = differentID
    use_xyzt = true
    function = 'z'
  []
[]

[Executioner]
  type = Steady
[]

[Reporters]
  [pt]
    type=ConstantReporter
    real_vector_names = 'pt1_x pt1_y pt1_z pt2_x pt2_y pt2_z'
    real_vector_values = '-0;
                           0;
                            -5;
                          1;
                          1;
                            5'
    outputs = none
  []

  [oneID_out]
    type=ClosestNodeToLine
    projection_tolerance= 1
    point_x1 =  pt/pt1_x
    point_y1 =  pt/pt1_y
    point_z1 =  pt/pt1_z
    point_x2 =  pt/pt2_x
    point_y2 =  pt/pt2_y
    point_z2 =  pt/pt2_z
    domain_name = oneID
    outputs = out
  []
  [differentID_out]
    type=ClosestNodeToLine
    projection_tolerance= 1
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    domain_name = differentID
    outputs = out
  []
[]

[Outputs]
  [out]
    type = JSON
    execute_system_information_on = none
  []
[]
