 [Mesh]
  type = FileMesh
  file = 50mX11m_single_horiz_fracture_mesh.e
  []

[Variables]
  [./temperature]
    initial_condition = 413.15
  [../]

  [./pressure]
    initial_condition = 2e6
  [../]

  [./disp_x]
  [../]

  [./disp_y]
  [../]

  [./disp_z]
  [../]

[]


[AuxVariables]
#----none of these aukernels are required for computation                                   ----#
#----                                                                                       ----#
#----velocity[x,y,z], density and viscosity of fluid, stress, strain, permeability, porosity----#
  [./v_x]
    order = CONSTANT
    family = MONOMIAL
    block = 2
  [../]
  [./v_y]
    order = CONSTANT
    family = MONOMIAL
    block = 2
  [../]
  [./density_water]
    order = CONSTANT
    family = MONOMIAL
    block = 2
  [../]
  [./viscosity_water]
    order = CONSTANT
    family = MONOMIAL
    block = 2
  [../]
#----seg faults pop up when this AuxKernel is active
#----working on trying to fix it
# [./tau_xx]
# order = CONSTANT
# family = MONOMIAL
# [../]
# [./tau_yy]
# order = CONSTANT
# family = MONOMIAL
# [../]
# [./tau_zz]
# order = CONSTANT
# family = MONOMIAL
# [../]
# [./tau_xy]
# order = CONSTANT
# family = MONOMIAL
# [../]
# [./tau_xz]
# order = CONSTANT
# family = MONOMIAL
# [../]
# [./tau_yz]
# order = CONSTANT
# family = MONOMIAL
# [../]
#[./s_xx]
#order = CONSTANT
#family = MONOMIAL
#[../]
#[./s_yy]
#order = CONSTANT
#family = MONOMIAL
#[../]
#[./s_zz]
#order = CONSTANT
#family = MONOMIAL
#[../]
#[./s_xy]
#order = CONSTANT
#family = MONOMIAL
#[../]
#[./s_xz]
#order = CONSTANT
#family = MONOMIAL
#[../]
#[./s_yz]
#order = CONSTANT
#family = MONOMIAL
#[../]
  [./perm]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./poro]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
#----Pressure: diffusion, time derivative / Temp.: diffusion, convection, time derivative----#
#----Mechanics: displacment, thermal strain, poro strain, time derivative                ----#
[./p_wmfp]
    type = WaterMassFluxPressure_PT
    variable = pressure
    block = '1 2 3'
  [../]
  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
    block = '1 2 3'
  [../]
  [./t_c]
    type = TemperatureConvection
    variable = temperature
    block = '1 2 3'
  [../]
  [./gravity]
    type = Gravity
    variable = disp_y
    component = 1
  [../]
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
  [./p_td]
    type = MassFluxTimeDerivative_PT
    variable = pressure
    block = '1 2 3'
  [../]
  [./t_td]
    type = TemperatureTimeDerivative
    variable = temperature
    block = '1 2 3'
  [../]
  [./temp_disp_x]
    type = SolidMechTempCoupleXFalcon
    variable = disp_x
    block = '1 3'
    temperature = temperature
  [../]
  [./temp_disp_y]
    type = SolidMechTempCoupleYFalcon
    variable = disp_y
    block = '1 3'
    temperature = temperature
  [../]
  [./temp_disp_z]
    type = SolidMechTempCoupleZFalcon
    variable = disp_z
    block = '1 3'
    temperature = temperature
  [../]
  [./y_td]
    type = SolidMechImplicitEuler
    variable = disp_y
  [../]
  [./x_td]
    type = SolidMechImplicitEuler
    variable = disp_x
  [../]
  [./z_td]
    type = SolidMechImplicitEuler
    variable = disp_y
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
[]

[AuxKernels]
#----none of these aukernels are required for computation----#
#----velocity[x,y,z], density and viscosity of fluid, stress, strain, permeability, porosity----#
  [./v_x]
    type = VelocityAux
    variable = v_x
    block = 2
  [../]
  [./v_y]
    type = VelocityAux
    variable = v_y
    component = 1
    block = 2
  [../]
  [./density_water]
    type = MaterialRealAux
    variable = density_water
    property = density_water
    block = 2
  [../]
  [./viscosity_water]
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
    block = 2
  [../]
#  [./comp_tau_xx]
#    type = StressStrainDamageComputeAux
#    variable = tau_xx
#  [../]
#----seg faults pop up when this AuxKernel is active
#----working on trying to fix it
# [./comp_tau_xx]
# type = StressStrainDamageComputeAux
# variable = tau_xx
# [../]
# [./comp_tau_yy]
# type = StressStrainDamageComputeAux
# variable = tau_yy
# component = 1
# [../]
# [./comp_tau_zz]
# type = StressStrainDamageComputeAux
# variable = tau_zz
# component = 2
# [../]
# [./comp_tau_xy]
# type = StressStrainDamageComputeAux
# variable = tau_xy
# component = 3
# [../]
# [./comp_tau_xz]
# type = StressStrainDamageComputeAux
# variable = tau_xz
# component = 4
# [../]
# [./comp_tau_yz]
# type = StressStrainDamageComputeAux
# variable = tau_yz
# component = 5
# [../]
#[./comp_s_xx]
#type = StressStrainDamageComputeAux
#variable = s_xx
#quantity = strain
#[../]
#[./comp_s_yy]
#type = StressStrainDamageComputeAux
#variable = s_yy
#component = 1
#quantity = strain
#[../]
#[./comp_s_zz]
#type = StressStrainDamageComputeAux
#variable = s_zz
#component = 2
#quantity = strain
#[../]
#[./comp_s_xy]
#type = StressStrainDamageComputeAux
#variable = s_xy
#component = 3
#quantity = strain
#[../]
#[./comp_s_xz]
#type = StressStrainDamageComputeAux
#variable = s_xz
#component = 4
#quantity = strain
#[../]
#[./comp_s_yz]
#type = StressStrainDamageComputeAux
#variable = s_yz
#component = 5
#quantity = strain
#[../]
  [./perm]
    type = MaterialRealAux
    variable = perm
    property = permeability
  [../]
  [./poro]
    type = MaterialRealAux
    variable = poro
    property = porosity
  [../]
[]

[BCs]
  [./outflow]
    type = OutFlowBC
    variable = temperature
    boundary = '5 6 7'
  [../]
  [./rock_t]
    type = NeumannBC
    variable = temperature
    boundary = '10 8'
  [../]
  [./water_t]
    type = SinDirichletBC
    variable = temperature
    boundary = 9
    initial = 413.15
    duration = 100
    final = 333.15
  [../]
  [./no_temp_flux]
    type = DirichletBC
    variable = temperature
    boundary = '4 1'
    value = 413.15
  [../]
  [./pressure_lo]
    type = DirichletBC
    variable = pressure
    boundary = '5 6 7'
    value = 2e6
  [../]
  [./no_y]
    type = PresetBC
    variable = disp_y
    boundary = '1 4'
    value = 0
  [../]
  [./no_z]
    type = PresetBC
    variable = disp_z
    boundary = '11 12'
    value = 0.0
  [../]
  [./no_x]
    type = PresetBC
    variable = disp_x
    boundary = '5 6 7 8 9 10'
    value = 0
  [../]
  [./pressure_hi_water]
    type = DirichletBC
    variable = pressure
    boundary = '8 9 10'
    value = 2.05e6
  [../]
[]

[Materials]
  [./water]
    type = Geothermal
    block = 2

    pressure = pressure
    temperature = temperature
    y_disp = disp_y
    x_disp = disp_x
    z_disp = disp_z

    temp_dependent_fluid_props = false

    permeability = 1e-11
    porosity = 1e-5
    youngs_modulus = 1.5e7
    thermal_conductivity = 0.58
    thermal_expansion = 4.2e-2
    gz = 0
    thermal_strain_ref_temp = 413.15
  [../]
  [./rock]
    type = Geothermal
    block = '1 3'

    pressure = pressure
    temperature = temperature
    x_disp = disp_x
    y_disp = disp_y
    z_disp = disp_z

    temp_dependent_fluid_props = false
    has_aperture_change_permeability = false

    permeability = 1e-17
    porosity = 0.01
    thermal_conductivity = 2.0
    thermal_expansion = 2e-5
    gy = 1.0
    gz = 0.0
    thermal_strain_ref_temp = 413.15
  [../]
[]


[Executioner]
  type = Transient
  num_steps = 1
  nl_rel_tol = 1e-10
  dt = 50
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart -snes_ls -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 basic 0.7'
  dtmax = 3e4
  dtmin = 0.0
  line_search = basic


  [./Adaptivity]
    initial_adaptivity = 1
    error_estimator = KellyErrorEstimator
    refine_fraction = 0.9
    coarsen_fraction = 0.001
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = PTD_CONST_STD_2D_1_out
  output_initial = false
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
