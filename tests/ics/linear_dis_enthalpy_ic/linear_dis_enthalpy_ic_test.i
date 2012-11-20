[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 4
  ny = 4
[]

[Variables]
  [./u]
  [../]
  [./v]
  [../]
[]

[Kernels]
  [./diff_u]
    type = Diffusion
    variable = u
  [../]
  [./diff_v]
    type = Diffusion
    variable = v
  [../]
[]

[BCs]
  [./right_v]
    type = DirichletBC
    variable = v
    boundary = 2
    value = 3
  [../]
  [./left_u]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 1
  [../]
[]

[Materials]
  [./constant]
    type = Constant
    block = 0
  [../]
[]

[Executioner]
  type = Steady
  petsc_options = -snes_mf_operator
  nl_rel_tol = 1e-15
  l_tol = 1e-12
[]

[Output]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
  perf_log = true
[]

[ICs]
  [./ic_v]
    slope = 5
    basevalue = 100
    variable = v
    type = LinearDisEnthalpyIC
    block = 0
  [../]
  [./ic_u]
    slope = 6
    basevalue = 4
    variable = u
    type = LinearDisEnthalpyIC
    block = 0
  [../]
[]

