[Mesh]
  type = FileMesh
  file = 3d_stochastic_course.e
[]

[Variables]
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10e6
  [../]
  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 473.15
  [../]
[]

[AuxVariables]
  [./permeability]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./p_td]
    type = MassFluxTimeDerivative_PT
    variable = pressure
  [../]
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  [./t_td]
    type = TemperatureTimeDerivative
    variable = temperature
  [../]
  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
  [../]
  [./t_c]
    type = TemperatureConvection
    variable = temperature
  [../]
[]

[AuxKernels]
  [./aux_permeability]
    type = StochasticFieldAux
    variable = permeability
    file_name = IESE_Permeability.txt
  [../]
[]

[BCs]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 10e6
  [../]
  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 373.15
  [../]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 1e6
  [../]
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 473.15
  [../]
[]

[Materials]
active = 'rock'
  [./StochasticGeothermalMaterial]
    block = 1
    solid_mechanics = false
    heat_transport = true
    fluid_flow = true
    chemical_reactions = false
    pressure = pressure
    temperature = temperature
    permeability = permeability
    temp_dependent_fluid_props = false
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
  [../]

  [./rock]
    type = StochasticGeothermal
    block = 1
    pressure = pressure
    temperature = temperature
    temp_dependent_fluid_props = false
    permeability = permeability
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 5
  dt = 2500000.0
  nl_abs_tol = 1e-6

  solve_type = 'NEWTON'

  [./Quadrature]
    type = Trap
  [../]
[]

[Outputs]
  file_base = out_const_dens_visc
  output_initial = true
  exodus = true
  [./console]
   type = Console
   perf_log = true
   linear_residuals = true
  [../]
[]
