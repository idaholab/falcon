[Mesh]
  dim = 2
  file = square.e
[]

[Variables]
  names = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  names = 'one five coupled coupled_nl'

  [./one]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./five]
    order = FIRST
    family = LAGRANGE
  [../]

  [./coupled]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./coupled_nl]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  names = 'diff force'

  [./diff]
    type = Diffusion
    variable = u
  [../]

  #Coupling of nonlinear to Aux
  [./force]
    type = CoupledForce
    variable = u
    coupled_to = 'one'
    coupled_as = 'v'
  [../]
[]

[AuxKernels]
  names = 'constant coupled  coupled_nl'

  #Simple Aux Kernel
  [./constant]
    variable = one
    type = ConstantAux
    value = 1
  [../]

  #Shows coupling of Element to Nodal
  [./coupled]
    variable = coupled
    type = CoupledAux
    value = 2
    coupled_to = 'five'
    coupled_as = 'coupled'
  [../]

  #Shows coupling of Element to non-linear
  [./coupled_nl]
    variable = coupled_nl
    type = CoupledAux
    value = 2
    coupled_to = 'u'
    coupled_as = 'coupled'
  [../]
[]

[BCs]
  names = 'left right'

  [./left]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0
  [../]

  [./right]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 1
  [../]
[]

[AuxBCs]
  names = 'five'
  
  [./five]
    type = ConstantAux
    variable = five
    boundary = '1 2'
    value = 5
  [../]
[]

[Materials]
  names = constant

  [./constant]
    type = Constant
    block = 1
  [../]
[]

[Execution]
  type = Steady
  perf_log = true
  petsc_options = '-snes_mf_operator'
[]

[Output]
  file_base = out
  interval = 1
  exodus = true
[]
   
    
