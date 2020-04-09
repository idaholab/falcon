#### FALCON test case: PT_TH_elder
#### Author: Yidong Xia (INL)
#### Description:
#### P-T based coupled mass & energy balance –--
#### thermally induced buoyant convection and instability
#### (using water-steam EOS)

############################################################
[Mesh]
  file = mesh.e
[]
############################################################
[Variables]
  [./P]
    initial_condition = 101325
  [../]
[]
############################################################
[Kernels]
  [./P_mass_residual]
    type = PTMassResidual
    variable = P
  [../]
[]
############################################################
[BCs]
  [./P_top]
    type = DirichletBC
    variable = P
    boundary = '3'
    value = 101325
  [../]
[]
############################################################
[Materials]
  [./porous]
    type = PTGeothermal
    block = 1

    pressure = P
    #temperature = T

    fluid_property_formulation = 'constant'
    stabilizer = 'none'

    permeability         = 1.0e-10   # default = 1.000e-12
    porosity             = 0.4       # default = 0.300e+00
    compressibility      = 4.0e-10   # default = 1.000e-05
    density_rock         = 2480      # default = 2.500e+03
    density_water        = 999.526   # default = 1.000e+03
    viscosity_water      = 0.001     # default = 1.200e-04
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
  type = Steady

  solve_type = 'PJFNK' # default = PJFNK | NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type
                         -ksp_gmres_restart -snes_ls
                         -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 cubic 0.7'

  l_max_its  = 400
  l_tol      = 1e-12
  nl_max_its = 15
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-7
[]
############################################################
[Outputs]
  file_base      = out1
  exodus         = true
  [./console]
    type = Console
    output_linear = true
    output_nonlinear = true
  [../]
[]
############################################################
