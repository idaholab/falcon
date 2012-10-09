[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  nz = 2
[]

[Variables]
  [./u]
    block = 0
  [../]
[]

[AuxVariables]
  [./const_dens]
    block = 0
  [../]
  [./const_visc]
    block = 0
  [../]
[]

[Functions]
  [./sin_func]
    type = ParsedFunction
    value = sin(x)
    vars = x
  [../]
[]

[Kernels]
  [./diffused]
    type = Diffusion
    variable = u
    block = 0
  [../]
[]

[AuxKernels]
  [./constant_density]
    type = ConstantDensityAux
    variable = const_dens
    block = 0
  [../]
  [./constant_viscosity]
    type = ConstantViscosityAux
    variable = const_visc
    viscosity_water = 0.12e-3
    block = 0
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 1
  [../]
  [./right]
    type = FunctionDirichletBC
    variable = u
    boundary = right
    function = sin_func
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 3
  dt = 1
[]

[Output]
  exodus = true
[]

