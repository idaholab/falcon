# This is a MOOSE input file to remove duplicate elements from sidesets created by the DFN gmsh script
# The gmsh script creates a sideset called disk1 that contains faces from the volumetric elements
# on both sides of the fracture.
# This creates a duplicate elements in the sideset that need to be removed.
# This script also has the mesh generator to put the sideset into its own lower Dimensional block.
# The area of the the seven sided paralelegram is checked.
# without removing the duplicate sideset, this value is doubled.

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = example1.msh
  []
  [rmDuplicateSS]
    input = fmg
    type = RemoveDuplicateFacesFromSidesets
  []
  [fracture]
    input = rmDuplicateSS
    type = LowerDBlockFromSidesetGenerator
    new_block_id = 11
    new_block_name = 'fracture'
    sidesets = 'disk1'
  []
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Steady
[]
[Functions]
  [unity]
    type = ParsedFunction
    expression = 1
  []
[]

[Postprocessors]
  [unity]
    type = FunctionElementIntegral
    function = 'unity'
    execute_on = 'initial'
    block = 11
  []
  [compare]
    type = ParsedPostprocessor
    expression = 'abs(unity-0.6841)'
    pp_names = 'unity'
  []
[]

[UserObjects]
  [terminate]
    type = Terminator
    expression = 'compare > 1e-4'
    error_level = ERROR
    message = 'The area of the seven sided polygon disk is 0.6841'
  []
[]

[Outputs]
[]
