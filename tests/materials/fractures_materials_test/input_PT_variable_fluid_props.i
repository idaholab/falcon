# Testing the FracturesGeothermal material with a simple fluid flow and
# heat transport problem.  A very course mesh and fracture distribution is
# used to make the test run faster.  Constant pressure gradient from top
# to bottom, initial temp of 50 C with a 100 C constant BC applied to the
# top of the domain.  Temp front convects faster through the fracture.  This
# test is the same as input_PT_const_fluid_props.i but with variable fluid
# properties and it is transient.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 5
  ny = 14
  xmax = 5
  ymax = 14
[]

[Variables]
  [./pressure]
    initial_condition = 10e6
    scaling = 1e6
  [../]
  [./temperature]
    initial_condition = 323.15
  [../]
[]

[AuxVariables]
  [./fractures]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dens]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./visc]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
  [../]
  [./t_c]
    type = TemperatureConvection
    variable = temperature
  [../]
  [./t_td]
    type = TemperatureTimeDerivative
    variable = temperature
  [../]
[]

[AuxKernels]
  [./fractures]
    type = StochasticFieldAux
    variable = fractures
    file_name = fractures_small.txt
  [../]
  [./vx]
    type = VelocityAux
    variable = vx
  [../]
  [./vy]
    type = VelocityAux
    variable = vy
    component = 1
  [../]
  [./dens]
    type = MaterialRealAux
    variable = dens
    property = density_water
  [../]
  [./visc]
    type = MaterialRealAux
    variable = visc
    property = viscosity_water
  [../]
[]

[BCs]
  [./top_p]
    type = DirichletBC
    variable = pressure
    boundary = top
    value = 10e6
  [../]
  [./bottom_p]
    type = DirichletBC
    variable = pressure
    boundary = bottom
    value = 9.99e6
  [../]
  [./top_t]
    type = DirichletBC
    variable = temperature
    boundary = top
    value = 373.15
  [../]
  [./bottom_t]
    type = DirichletBC
    variable = temperature
    boundary = bottom
    value = 323.15
  [../]
[]

[Materials]
  [./rock]
    type = FracturesGeothermal
    block = 0
    pressure = pressure
    fractures = fractures
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    fracture_density = 1000
    fracture_porosity = 0.90
    fracture_permeability = 1e-8
    fracture_thermal_expansion = 1e-6
    fracture_youngs_modulus = 1.5e+7
    fracture_thermal_conductivity = 580
    fracture_t_ref = 140
    matrix_permeability = 1e-17
    matrix_porosity = 0.01
    matrix_thermal_expansion = 2e-05
    matrix_t_ref = 140
    matrix_thermal_conductivity = 2.0
    has_aperture_change_permeability = false
    temperature = temperature
    water_steam_properties = water_steam_properties
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 20
  dt = 10000

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

[]

[Output]
  file_base = out_PT_variable_fluid_props
  output_initial = true
  exodus = true
  print_out_info = true
[]

