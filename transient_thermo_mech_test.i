#Run with 4 procs

[Mesh]
  dim = 3
  file = shale_cyl.e
[]

[Variables]
  names = 'x_disp y_disp z_disp temp'

  #names = 'temp'

  [./x_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./y_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./z_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 300
  [../]
[]

[Kernels]
  names = 'solid_x solid_y solid_z solid_x_ie solid_y_ie solid_z_ie solid_couple_x solid_couple_y solid_couple_z heat heat_ie'
#  names = 'heat'

  [./solid_x]
    type = SolidMechX
    variable = x_disp
    coupled_to = 'y_disp z_disp'
    coupled_as = 'y z'
  [../]

  [./solid_y]
    type = SolidMechY
    variable = y_disp
    coupled_to = 'x_disp z_disp'
    coupled_as = 'x z'
  [../]

  [./solid_z]
    type = SolidMechZ
    variable = z_disp
    coupled_to = 'x_disp y_disp'
    coupled_as = 'x y'
  [../]

  [./solid_x_ie]
    type = SolidMechImplicitEuler
    variable = x_disp
  [../]

  [./solid_y_ie]
    type = SolidMechImplicitEuler
    variable = y_disp
  [../]

  [./solid_z_ie]
    type = SolidMechImplicitEuler
    variable = z_disp
  [../]

  [./solid_couple_x]
    type = SolidMechTempCoupleX
    variable = x_disp
    coupled_to = 'temp'
    coupled_as = 'temp'
  [../]

  [./solid_couple_y]
    type = SolidMechTempCoupleY
    variable = y_disp
    coupled_to = 'temp'
    coupled_as = 'temp'
  [../]

  [./solid_couple_z]
    type = SolidMechTempCoupleZ
    variable = z_disp
    coupled_to = 'temp'
    coupled_as = 'temp'
  [../]

  [./heat]
    type = HeatConduction
    variable = temp
  [../]

  [./heat_ie]
    type = HeatConductionImplicitEuler
    variable = temp
  [../]
[]

[BCs]
  names = 'heater bottom_y side_x side_y side_z bore_x bore_z'
#bottom_temp top_temp left_temp'
  [./heater]
    type = NeumannBC
    variable = temp
    boundary = 6
    value = 700
  [../]

  [./bottom_y]
    type = DirichletBC
    variable = y_disp
    boundary = 1
    value = 0.0
  [../]

  [./side_x]
    type = DirichletBC
    variable = x_disp
    boundary = 3
    value = 0.0
  [../]

  [./side_y]
    type = DirichletBC
    variable = y_disp
    boundary = 3
    value = 0.0
  [../]

  [./side_z]
    type = DirichletBC
    variable = z_disp
    boundary = 3
    value = 0.0
  [../]

  [./bore_x]
    type = DirichletBC
    variable = x_disp
    boundary = '4 5 6 7'
    value = 0.0
  [../]

  [./bore_z]
    type = DirichletBC
    variable = z_disp
    boundary = '4 5 6 7'
    value = 0.0
  [../]
[]

[Materials]
  names = 'uinta green_river mahogany lower_green_river'
  
  [./uinta]
    type = Constant
    block = 1
    coupled_to = 'temp'
    coupled_as = 'temp'
    thermal_conductivity = 1.0
    youngs_modulus = 1.0e10
    poissons_ratio = .3
    thermal_expansion = 1e-5
    density = 2.7e3
    specific_heat = 1.0e3
    t_ref = 300
  [../]

  [./green_river]
    type = Constant
    block = 2
    coupled_to = 'temp'
    coupled_as = 'temp'
    thermal_conductivity = 1.0
    youngs_modulus = 1.0e10
    poissons_ratio = .3
    thermal_expansion = 1e-5
    density = 2.7e3
    specific_heat = 1.0e3
    t_ref = 300
  [../]

  [./mahogany]
    type = Constant
    block = 3
    coupled_to = 'temp'
    coupled_as = 'temp'
    thermal_conductivity = 1.0
    youngs_modulus = 1.0e10
    poissons_ratio = .3
    thermal_expansion = 1e-5
    density = 2.7e3
    specific_heat = 1.0e3
    t_ref = 300
  [../]

  [./lower_green_river]
    type = Constant
    block = 4
    coupled_to = 'temp'
    coupled_as = 'temp'
    thermal_conductivity = 1.0
    youngs_modulus = 1.0e10
    poissons_ratio = .3
    thermal_expansion = 1e-5
    density = 2.7e3
    specific_heat = 1.0e3
    t_ref = 300
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator'
  petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value =  'hypre    boomeramg 100'

  l_max_its =  60
  l_tol =  1e-5

  nl_max_its =  10
  nl_rel_tol =  1e-5

  [./Transient]
    start_time = 0.0
    num_steps = 100
    dt = 10000000
  [../]
[]

[Output]
  file_base = out
  interval = 1
  output_initial = true
  exodus = true
[]
   
    
