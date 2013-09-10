[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  zmax = 0.1
[]

[Variables]
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 1.e7
  [../]
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

[Kernels]
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  [./poro_x]
    type = SolidMechPoroCoupleX
    variable = disp_x
    pressure = pressure
  [../]
  [./poro_y]
    type = SolidMechPoroCoupleY
    variable = disp_y
    pressure = pressure
  [../]
  [./poro_z]
    type = SolidMechPoroCoupleZ
    variable = disp_z
    pressure = pressure
  [../]
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

[BCs]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 1e7
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 1e6
  [../]
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
  active = 'GeothermalMaterial'
  [./GeothermalMaterial]
    block = 0
    solid_mechanics = true
    heat_transport = false
    fluid_flow = true
    chemical_reactions = false
    pressure = pressure
    temp_dependent_fluid_props = false
    x_disp = disp_x
    y_disp = disp_y
    z_disp = disp_z
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  print_linear_residuals = true

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Output]
  file_base = out
  output_initial = true
  exodus = true
  print_out_info = true
[]

