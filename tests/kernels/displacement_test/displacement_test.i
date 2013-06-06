[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  zmax = 0.1
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.0
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.0
  [../]
  [./disp_z]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.0
  [../]
[]

[AuxVariables]
  [./tau_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tau_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tau_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tau_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tau_xz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tau_yz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s_xz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s_yz]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  active = 'div_y div_x div_z gravity_z'
  [./div_x]
    type = SolidMechXFalcon
    variable = disp_x
    y = disp_y
    z = disp_z
  [../]
  [./div_y]
    type = SolidMechYFalcon
    variable = disp_y
    x = disp_x
    z = disp_z
  [../]
  [./div_z]
    type = SolidMechZFalcon
    variable = disp_z
    x = disp_x
    y = disp_y
  [../]
  [./gravity_z]
    type = Gravity
    variable = disp_z
    component = 2
  [../]
[]

[AuxKernels]
  [./comp_tau_xx]
    type = StressStrainDamageComputeAux
    variable = tau_xx
    quantity = stress
    component = 0
  [../]
  [./comp_tau_yy]
    type = StressStrainDamageComputeAux
    variable = tau_yy
    quantity = stress
    component = 1
  [../]
  [./comp_tau_zz]
    type = StressStrainDamageComputeAux
    variable = tau_zz
    quantity = stress
    component = 2
  [../]
  [./comp_tau_xy]
    type = StressStrainDamageComputeAux
    variable = tau_xy
    quantity = stress
    component = 3
  [../]
  [./comp_tau_xz]
    type = StressStrainDamageComputeAux
    variable = tau_xz
    quantity = stress
    component = 4
  [../]
  [./comp_tau_yz]
    type = StressStrainDamageComputeAux
    variable = tau_yz
    quantity = stress
    component = 5
  [../]
  [./comp_s_xx]
    type = StressStrainDamageComputeAux
    variable = s_xx
    quantity = strain
    component = 0
  [../]
  [./comp_s_yy]
    type = StressStrainDamageComputeAux
    variable = s_yy
    quantity = strain
    component = 1
  [../]
  [./comp_s_zz]
    type = StressStrainDamageComputeAux
    variable = s_zz
    quantity = strain
    component = 2
  [../]
  [./comp_s_xy]
    type = StressStrainDamageComputeAux
    variable = s_xy
    quantity = strain
    component = 3
  [../]
  [./comp_s_xz]
    type = StressStrainDamageComputeAux
    variable = s_xz
    quantity = strain
    component = 4
  [../]
  [./comp_s_yz]
    type = StressStrainDamageComputeAux
    variable = s_yz
    quantity = strain
    component = 5
  [../]
[]

[BCs]
  [./top_w]
    type = NeumannBC
    variable = disp_z
    boundary = front
    value = -12.5e6
  [../]
  [./bot_w]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0
  [../]
  [./side_confine_u1]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./side_confine_u2]
    type = NeumannBC
    variable = disp_x
    boundary = right
    value = -25e6
  [../]
  [./side_confine_v1]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./side_confine_v2]
    type = NeumannBC
    variable = disp_y
    boundary = top
    value = -25e6
  [../]
[]

[Materials]
  [./frac]
    type = SolidMechanics
    block = 0
    x_disp = disp_x
    y_disp = disp_y
    z_disp = disp_z
  [../]
[]

[Executioner]
  type = Steady
  petsc_options = -snes_mf_operator
[]

[Output]
  file_base = out
  output_initial = true
  exodus = true
  print_out_info = true
[]

