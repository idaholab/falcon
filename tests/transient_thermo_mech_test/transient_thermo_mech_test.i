#Run with 4 procs

[Mesh]
  dim = 3
  file = cube.e
[]

[Variables]
  active = 'x_disp y_disp z_disp temp'

  #active = 'temp'

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
  [../]
[]

[Kernels]
  active = 'solid_x solid_y solid_z solid_x_ie solid_y_ie solid_z_ie solid_couple_x solid_couple_y solid_couple_z heat heat_ie'
#  active = 'heat'

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
    coupled_as = 'temperature'
  [../]

  [./solid_couple_y]
    type = SolidMechTempCoupleY
    variable = y_disp
    coupled_to = 'temp'
    coupled_as = 'temperature'
  [../]

  [./solid_couple_z]
    type = SolidMechTempCoupleZ
    variable = z_disp
    coupled_to = 'temp'
    coupled_as = 'temperature'
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
  active = 'bottom_x bottom_y bottom_z bottom_temp'
#bottom_temp top_temp left_temp'

  [./top_y]
    type = NeumannBC
    variable = y_disp
    boundary = 2
    value = 1.0
  [../]

  [./bottom_x]
    type = DirichletBC
    variable = x_disp
    boundary = 1
    value = 0.0
  [../]

  [./bottom_y]
    type = DirichletBC
    variable = y_disp
    boundary = 1
    value = 0.0
  [../]

  [./bottom_z]
    type = DirichletBC
    variable = z_disp
    boundary = 1
    value = 0.0
  [../]

  [./bottom_temp]
    type = DirichletBC
    variable = temp
    boundary = 1
    value = 10.0
  [../]
[]

[Materials]
  active = 'constant'
  
  [./constant]
    type = ThermalPoroElastic
    block = 1
    coupled_to = 'temp        x_disp y_disp z_disp'
    coupled_as = 'temperature x_disp y_disp z_disp'
    thermal_conductivity = 1.0
    rho_r                = 1.0
    rock_specific_heat   = 1.0
    youngs_modulus = 1.0
    poissons_ratio = .3
    thermal_expansion = 1e-5
    t_ref = 300
  [../]
[]

[Execution]
  type = Transient
  perf_log = true
  petsc_options = '-snes_mf_operator'

  nl_rel_tol = 1e-14

  [./Transient]
    start_time = 0.0
    num_steps = 3
    dt = 1
  [../]
[]

[Output]
  file_base = out
  interval = 1
  output_initial = true
  exodus = true
[]
   
    
