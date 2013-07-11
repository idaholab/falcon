[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  xmax = 10
[]

[Variables]
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10e6
  [../]
[]

[Kernels]
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
[]

[BCs]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 10e6
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 1e6
  [../]
[]

[Materials]
  [./GeothermalMaterial]
    block = 0
    solid_mechanics = false
    heat_transport = true
    fluid_flow = true
    chemical_reactions = false
    pressure = pressure
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    porosity = 0.5
    permeability = 1.0e-14
    temp_dependent_fluid_props = false
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 2
  dt = 10.0
  [./Quadrature]
    type = Trap
  [../]
[]

[Output]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

