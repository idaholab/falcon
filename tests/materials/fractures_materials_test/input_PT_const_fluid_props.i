# Testing the FracturesGeothermal material with a simple fluid flow and
# heat transport problem.  A very course mesh and fracture distribution is
# used to make the test run faster.  Constant pressure gradient from top
# to bottom, initial temp of 50 C with a 100 C constant BC applied to the
# top of the domain.  Temp front convects faster through the fracture.  This
# test is the same as input_PT_variable_fluid_props.i but with constant fluid
# properties and it is a steady-state problem.

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
    order = FIRST
    family = LAGRANGE
    initial_condition = 10e6
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
[]

[Kernels]
  [./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
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
    temp_dependent_fluid_props = false
    has_aperture_change_permeability = false
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'
[]

[Outputs]
  file_base = out_PT_const_fluid_props
  output_initial = true
  exodus = true
  console = true
[]
