#The line segment defined in the constant reporter intersects exactly at the
#element centroid at the correct location with this mesh density.
#The mesh used in these simulations is created using the following MeshGeneratorMesh and auxKernel

# [Mesh]
# type = MeshGeneratorMesh
# [dfn1]
#   type = GeneratedMeshGenerator
#   dim = 2
#   nx = 5
#   ny = 5
#   elem_type = QUAD4
# []
# [dfn2]
#   type = GeneratedMeshGenerator
#   dim = 2
#   nx = 5
#   ny = 5
#   elem_type = QUAD4
# []
# [dfn2_shift]
#   type = TransformGenerator
#   input = dfn2
#   transform = TRANSLATE
#   vector_value = '0 0 -4'
# []
# [dfn3]
#   type = GeneratedMeshGenerator
#   dim = 2
#   nx = 5
#   ny = 5
#   elem_type = QUAD4
# []
# [dfn3_shift]
#   type = TransformGenerator
#   input = dfn3
#   transform = TRANSLATE
#   vector_value = '0 0 2'
# []
# [dfns]
#   type = MeshCollectionGenerator
#   inputs = 'dfn1 dfn2_shift dfn3_shift'
# []
# []
# [AuxKernels]
# [differentID]
#   type = ParsedAux
#   variable = differentID
#   use_xyzt = true
#   function = 'z'
#   execute_on = INITIAL
# []
# []

[Mesh]
  [meshFromAboveCommands]
    type = FileMeshGenerator
    file = 'planes_with_fracIds.e'
    use_for_exodus_restart = true
  []
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
    initial_from_file_var = differentID
    initial_from_file_timestep = 'LATEST'
  []
[]

[AuxKernels]
  [oneID]
    type = ConstantAux
    variable = oneID
    value = 5
  []
[]

[Executioner]
  type = Steady
[]

[Reporters]
  [pt]
    type = ConstantReporter
    real_vector_names = 'pt1_x pt1_y pt1_z pt2_x pt2_y pt2_z'
    real_vector_values = '-0;0;-5; 1;1;5'
    outputs = none
  []

  [oneID_out]
    type = ClosestElemsToLine
    projection_tolerance = 1
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    variable = oneID
    outputs = out
  []
  [differentID_out]
    type = ClosestElemsToLine
    projection_tolerance = 1
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    variable = differentID
    outputs = out
  []
  [noID_out]
    type = ClosestElemsToLine
    projection_tolerance = 0.2
    point_x1 = pt/pt1_x
    point_y1 = pt/pt1_y
    point_z1 = pt/pt1_z
    point_x2 = pt/pt2_x
    point_y2 = pt/pt2_y
    point_z2 = pt/pt2_z
    outputs = out
  []
[]

[Outputs]
  [out]
    type = JSON
    execute_system_information_on = none
  []
[]
