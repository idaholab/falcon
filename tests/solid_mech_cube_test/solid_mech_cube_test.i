#Run with 4 procs

[Mesh]
  dim = 3
  file = cube.e
[]

[Variables]
  active = 'x_disp y_disp z_disp'

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
[]

[Kernels]
  active = 'solid_x solid_y solid_z'

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
    coupled_to = 'y_disp x_disp'
    coupled_as = 'y x'
  [../]
[]

[BCs]
  active = 'y_force left bottom'

  [./y_force]
    type = NeumannBC
    variable = y_disp
    boundary = 2
    value = -1.0
  [../]

  [./left]
    type = DirichletBC
    variable = x_disp
    boundary = 3
    value = 0.0
  [../]

  [./bottom]
    type = DirichletBC
    variable = y_disp
    boundary = 1
    value = 0.0
  [../]

[]

[Materials]
  active = 'constant'
  
  [./constant]
    type = ThermalPoroElastic
    block = 1
    coupled_to = 'x_disp y_disp z_disp'
    coupled_as = 'x_disp y_disp z_disp'
    youngs_modulus = 1e6
    poissons_ratio = .3
  [../]
[]

[Executioner]
  type = Steady #Transient
  perf_log = true
  petsc_options = '-snes_mf -ksp_monitor'

  start_time = 0.0
  num_steps = 4
  dt = 0.000005
[]

[Output]
  file_base = out
  interval = 1
  output_initial = true
  exodus = true
#  gmv = true
#  tecplot = true
[]
   
    
