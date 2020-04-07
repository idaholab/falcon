#### FALCON test case: PT_TH_elder
#### Author: Yidong Xia (INL)
#### Description:
#### P-T based coupled mass & energy balance –--
#### thermally induced buoyant convection and instability
#### (using water-steam EOS)

############################################################
[Mesh]
  file = out1.e
[]
############################################################
[Variables]
  [./P]
    initial_from_file_var = P
    initial_from_file_timestep = 2
  [../]

  [./T]
    initial_condition = 285.15
  [../]
[]
############################################################
[Preconditioning]
  [./SMP]
    #type = FDP
    type = SMP
    off_diag_row    = 'P'
    off_diag_column = 'T'
  [../]
[]
############################################################
[AuxVariables]
  [./v_x]
    order = FIRST
    family = MONOMIAL
  [../]

  [./v_y]
    order = FIRST
    family = MONOMIAL
  [../]
[]
############################################################
[Kernels]
  [./P_time_derivative]
    type = PTMassTimeDerivative
    variable = P
  [../]

  [./P_mass_residual]
    type = PTMassResidual
    variable = P
    coupled_temperature = T
  [../]

  [./T_time_derivative]
    type = PTEnergyTimeDerivative
    variable = T
  [../]

  [./T_energy_residual]
    type = PTEnergyResidual
    variable = T
  [../]
[]
############################################################
[AuxKernels]
  [./vx]
    type = PTFluidVelocityAux
    variable = v_x
    component = 0
  [../]

  [./vy]
    type = PTFluidVelocityAux
    variable = v_y
    component = 1
  [../]
[]
############################################################
[BCs]
  #### 1 = bottom left half  (20 Celsius)
  #### 2 = bottom right half (12 Celsius)
  #### 3 = top boundary      (12 Celsius)

  [./P_top]
    type = DirichletBC
    variable = P
    boundary = '3'
    value = 101325
  [../]

  [./T_top]
    type = DirichletBC
    variable = T
    boundary = '3'
    value = 285.15
  [../]

  [./T_bottom1]
    type = DirichletBC
    variable = T
    boundary = '1'
    value = 293.15
  [../]

  [./T_bottom2]
    type = DirichletBC
    variable = T
    boundary = '2'
    value = 285.15
  [../]
[]
############################################################
[Materials]
  [./porous]
    type = PTGeothermal
    block = 1

    pressure = P
    temperature = T

    fluid_property_formulation = 'wseos'
    stabilizer = 'none'

    permeability         = 1.0e-10   # default = 1.000e-12
    porosity             = 0.4       # default = 0.300e+00
    compressibility      = 1.0e-10   # default = 1.000e-05
    density_rock         = 2480      # default = 2.500e+03
    density_water        = 999.526088   # default = 1.000e+03
    viscosity_water      = 0.0012389 # default = 1.200e-04
    specific_heat_rock   = 920       # default = 0.920e+03
    specific_heat_water  = 4180      # default = 4.186e+03
    thermal_conductivity = 1.0       # default = 2.500e+00
    gravity              = 9.80665   # default = 9.80665
    gravity_direction    = '0 -1 0'  # default = '0 0 -1'

    constant_pressure_gradient = '0 0 0'
  [../]
[]
############################################################
[Executioner]
  type = Transient

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type
                         -ksp_gmres_restart -snes_ls
                         -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 cubic 0.7'

  num_steps  = 1 # use 50000 for complete run
  dt         = 10000
  end_time   = 630720000
  l_max_its  = 30
  l_tol      = 1e-4 # could use 1e-3 for faster stepping
  nl_max_its = 15
  nl_rel_tol = 1e-4 # could use 1e-3 for faster stepping
  nl_abs_tol = 1e-7
[]
############################################################
[Outputs]
  file_base      = out2_wseos
  # use 100 for complete run
  exodus         = true
  [./console]
    type = Console
    output_linear = true
    output_nonlinear = true
  [../]
[]
############################################################
