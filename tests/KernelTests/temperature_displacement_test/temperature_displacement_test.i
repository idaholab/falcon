# ###############################################################################################################
# A simple themo-poro-elastic example for iceland injection
# injection of 50 degree water into 500x500x500 m^3 reservoir at rate of 24Kg/s (using Trap integration rule)
# initial reservoir pressure & temperature are 10MP and 200, respectively
# initial vetitial stress is set to be 25MP (~1km over burden), and initial horizontal confining stress is 12.5MP
# 
# !! a relatively large linear tolerance is used since mechanical deformation reach equilibrium very quickly
# !! the pressure also reach near steady state fairly quicky
# !! cooling zone also reaches quasi-steady at later stage: temperature change very slowing
# ###############################################################################################################

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 10
  zmax = 0.1
[]

[Variables]
  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 200.
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
  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
  [../]
  [./temp_x]
    type = SolidMechTempCoupleXFalcon
    variable = disp_x
    temperature = temperature
  [../]
  [./temp_y]
    type = SolidMechTempCoupleYFalcon
    variable = disp_y
    temperature = temperature
  [../]
  [./temp_z]
    type = SolidMechTempCoupleZFalcon
    variable = disp_z
    temperature = temperature
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
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 200.
  [../]
  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 10.
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
  [./frac]
    type = Geothermal
    block = 0
    water_steam_properties = water_steam_properties
    x_disp = disp_x
    y_disp = disp_y
    z_disp = disp_z
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
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
  petsc_options = -snes_mf_operator
  nl_rel_tol = 1e-12
[]

[Output]
  file_base = out
  output_initial = true
  exodus = true
  print_out_info = true
[]

