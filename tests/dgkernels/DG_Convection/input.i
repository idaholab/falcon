#### Author: Yidong Xia (Yidong.Xia@inl.gov)
#### Date: 08/27/2014
#### Description:
#### this test case checks the following kernels
#### DGConvection DGConvectionInflowBC DGConvectionOutflowBC

[Mesh]
  type = GeneratedMesh
  dim  = 3
  nx   = 100
  xmin = 0
  xmax = 100
[]

[Outputs]
  file_base      = out
  output_initial = true
  interval       = 1 # default 10
  exodus         = true
[]

[Executioner]
  type       = Transient
  solve_type = 'PJFNK'
  num_steps  = 5 # default 1000
  dt         = 400
  nl_rel_tol = 1e-4
[]

[Variables]
  [./temperature]
    order = FIRST
    family = L2_LAGRANGE # L2_LAGRANGE | MONOMIAL
    [./InitialCondition]
      type = ConstantIC
      value = 463.15
    [../]
  [../]
[]

[Kernels]
  [./tran]
    type = TimeDerivative
    variable = temperature
  [../]
  [./conv]
    type = Convection
    variable = temperature
    velocity = '1e-4 0 0'
  [../]
[]

[DGKernels]
  [./dgconv]
    type = DGConvection
    variable = temperature
    velocity = '1e-4 0 0'
  [../]
[]

[BCs]
  [./left]
    type = DGConvectionInflowBC
    variable = temperature
    boundary = 'left'
    value = 373.15
    velocity = '1e-4 0 0'
  [../]
  [./right]
    type = DGConvectionOutflowBC
    variable = temperature
    boundary = 'right'
    velocity = '1e-4 0 0'
  [../]
[]
