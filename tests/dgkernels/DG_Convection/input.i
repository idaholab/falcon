#### Author : Yidong Xia (Yidong.Xia@inl.gov)
#### Created: 08/20/2014
#### Description: 
#### This test case is designed to validate and verify 
#### the DGConvection kernel by performing a simulation of  
#### a transient scalar linear convection for a step function.
#### You will observe the non-physical oscillations 
#### (over- and under-shoots) at the vicinity of discontinuities.
#### Nevertheless, the wave propagation is stable
#### without any special treatment for stabilization.

[Mesh]
  type = GeneratedMesh
  dim  = 3
  nx   = 100
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 0.1
  zmin = 0.0
  zmax = 0.1
[]

[Outputs]
  file_base      = out
  output_initial = true
  interval       = 1 # 1 | 10
  exodus         = true
[]

[Executioner]
  type       = Transient
  solve_type = 'PJFNK'
  num_steps  = 10 # 10 | 3000
  dt         = 0.0002
  nl_rel_tol = 1e-4
[]

[Variables]
  [./u]
    order = FIRST # FIRST | SECOND
    family = L2_LAGRANGE # L2_LAGRANGE | MONOMIAL
    [./InitialCondition]
      type = ConstantIC
      value = 0.0
    [../]
  [../]
[]

[Kernels]
  [./tran]
    type = TimeDerivative
    variable = u
  [../]
  [./conv]
    type = Convection
    variable = u
    velocity = '1.0 0.0 0.0'
  [../]
[]

[DGKernels]
  [./dgconv]
    type = DGConvection
    variable = u
    velocity = '1.0 0.0 0.0'
  [../]
[]

[BCs]
  [./left]
    type = NeumannBC
    variable = u
    boundary = 'left'
    value = 1.0
  [../]
  [./right]
    type = NeumannBC
    variable = u
    boundary = 'right'
    value = 0.0
  [../]
[]
