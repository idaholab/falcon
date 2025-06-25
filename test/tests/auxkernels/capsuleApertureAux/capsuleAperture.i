[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 20
    ny = 20
    nz = 20
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
  [aperture_fracture]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [aperture_fracture]
    type=CapsuleApertureAux
    variable = aperture_fracture
    start_point ='100 100 125'
    end_point ='100 100 75'
    a_max = '1e-4'
    a_min = '1e-5'
    midpoint_of_sigmoid = 50
    slope_at_midpoint = 0.05
    execute_on = 'INITIAL'
  []
[]

[VectorPostprocessors]
  [aperture_line_sampler]
    type = LineValueSampler
    variable = aperture_fracture
    start_point = '100 100 0'
    end_point = '100 100 200'
    num_points = 50
    sort_by = z
  []
[]
[Executioner]
  type = Steady
[]

[Outputs]
  csv=true
[]
