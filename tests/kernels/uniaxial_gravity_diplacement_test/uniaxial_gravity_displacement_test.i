# Uniaxial gravity diplacement test:
# 
# This test varifies the gravity/solid mechanics kerenels
# and stress/strain auxkernel.
# Boundary conditions are zero displacement in the y and 
# z directions.  Acceleration due to gravity = 10m/s2,
# Young's modulus = 10 GPa, and density_rock = 2000 kg/m3.
# 
# The analytic solution for the displacment at x = 10m is:
# 
# u(x) = rho * g * h * x / E = rho * g * x^2 / E
# = (2000) (10) (10)^2 / 1e10
# = 0.0002 m
# 
# 
# The analytic solution for the stress along at x = 10m is:
# 
# S(x) = rho * g * x = (2000) (10) (10) = 2e5 Pa
# 
# The analylic solution for the strain at x = 10m is:
#
# Tau(x) = u(x) / x = 0.0002 / 10 = 2e-5
#

[Mesh]
  type = GeneratedMesh
  dim = 3
  nz = 10
  zmax = 10
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
  [./gravity_x]
    type = Gravity
    variable = disp_x
    component = 0
    block = 0
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
  [./no_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'left right'
    value = 0
  [../]
  [./no_x]
    type = DirichletBC
    variable = disp_x
    boundary = back
    value = 0
  [../]
  [./no_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom top'
    value = 0
  [../]
[]

[Materials]
  [./frac]
    type = SolidMechanics
    block = 0
    x_disp = disp_x
    y_disp = disp_y
    z_disp = disp_z
    density_rock = 2000
    gravity = 10
    poissons_ratio = 0
    gz = 0
    gx = 1
    youngs_modulus = 1e10
  [../]
[]

[Executioner]
  type = Steady
[]

[Output]
  file_base = out
  output_initial = true
  exodus = true
  print_out_info = true
[]

