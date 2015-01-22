[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 5
  ny = 14
  xmax = 5
  ymax = 14
[]

[Variables]
  [./disp_x]
    scaling = 1e-9
  [../]
  [./disp_y]
    scaling = 1e-9
  [../]
  [./disp_z]
    scaling = 1e-9
  [../]
[]

[AuxVariables]
  [./fractures]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./disp_x]
    type = SolidMechXFalcon
    variable = disp_x
    y = disp_y
    z = disp_z
  [../]
  [./disp_y]
    type = SolidMechYFalcon
    variable = disp_y
    x = disp_x
    z = disp_z
  [../]
  [./disp_z]
    type = SolidMechZFalcon
    variable = disp_z
    x = disp_x
    y = disp_y
  [../]
[]

[AuxKernels]
  [./fractures]
    type = StochasticFieldAux
    variable = fractures
    file_name = fractures_small.txt
  [../]
[]

[BCs]
  [./x_confine]
    type = DirichletBC
    variable = disp_x
    boundary = 'left right'
    value = 0.0
  [../]
  [./z_confine]
    type = DirichletBC
    variable = disp_z
    boundary = 'back front'
    value = 0.0
  [../]
  [./top_y_confine]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = -0.01
  [../]
  [./no_y_bottom]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
[]

[Materials]
  [./matrix]
    type = FracturesGeothermal
    block = 0
    x_disp = disp_x
    y_disp = disp_y
    fractures = fractures
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 0.0
    fracture_thermal_conductivity = 580
    fracture_thermal_expansion = 2e-5
    fracture_youngs_modulus = 1.5e+7
    fracture_thermal_strain_ref_temp = 413.15
    matrix_thermal_conductivity = 2.0
    matrix_thermal_expansion = 2e-05
    matrix_thermal_strain_ref_temp = 413.15
    temp_dependent_fluid_props = false
  [../]
[]

[Executioner]
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 401 '

  line_search = 'none'

  nl_abs_tol = 1e-5
[]

[Outputs]
  file_base = out_mechanics
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
