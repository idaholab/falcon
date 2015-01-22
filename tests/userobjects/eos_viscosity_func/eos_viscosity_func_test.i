# This test is used to verify the output of the viscosity function in the
# WaterSteamEOS UserObject.  A combination of incremented densities and
# temperatures are inputed into the EOSViscosityFuncPPS and the viscosity
# (water/steam) is outputted.  Saturated mix viscosity will be added to
# the test soon. The results are compared to a csv file which contains
# IAWPS97 water and steam formulation data

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./u]
  [../]
[]

[Functions]
  [./temperature_function1]
    type = ParsedFunction
    value = 10+20*t
  [../]
 [./temperature_function2]
 type = ParsedFunction
 value = 180+10*t
 [../]
 [./temperature_function3]
 type = ParsedFunction
 value = 150+10*t
 [../]
 [./temperature_function4]
 type = ParsedFunction
 value = 120+20*t
 [../]
 [./temperature_function5]
 type = ParsedFunction
 value = 100+20*t
 [../]
 [./temperature_function6]
 type = ParsedFunction
 value = 60+20*t
 [../]
 [./temperature_function7]
 type = ParsedFunction
 value = 30+20*t
 [../]
 [./temperature_function8]
 type = ParsedFunction
 value = 80+20*t
 [../]
 []

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
  [./time_yipee]
    type = TimeDerivative
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[UserObjects]
  [./water_steam_properties]
    type = WaterSteamEOS
  [../]
[]

[Postprocessors]
 [./viscositywater_1]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 1003
 temperature_value = temperature_function1
 water_phase = true
 [../]
 [./viscositywater_2]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 1000
 temperature_value = temperature_function1
 water_phase = true
 [../]
 [./viscositywater_3]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 997
 temperature_value = temperature_function7
 water_phase = true
 [../]
 [./viscositywater_4]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 993
 temperature_value = temperature_function6
 water_phase = true
 [../]
 [./viscositywater_5]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 990
 temperature_value = temperature_function6
 water_phase = true
 [../]
 [./viscositywater_6]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 987
 temperature_value = temperature_function6
 water_phase = true
 [../]
 [./viscositywater_7]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 983
 temperature_value = temperature_function8
 water_phase = true
 [../]
 [./viscositywater_8]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 980
 temperature_value = temperature_function8
 water_phase = true
 [../]
 [./viscositywater_9]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 977
 temperature_value = temperature_function8
 water_phase = true
 [../]
 [./viscositysteam_1]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 5
 temperature_value = temperature_function2
 water_phase = false
 [../]
 [./viscositysteam_2]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 2.5
 temperature_value = temperature_function3
 water_phase = false
 [../]
 [./viscositysteam_3]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 1
 temperature_value = temperature_function4
 water_phase = false
 [../]
 [./viscositysteam_4]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 0.75
 temperature_value = temperature_function4
 water_phase = false
 [../]
 [./viscositysteam_5]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 0.5
 temperature_value = temperature_function5
 water_phase = false
 [../]
 [./viscositysteam_6]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 0.25
 temperature_value = temperature_function8
 water_phase = false
 [../]
 [./viscositysteam_7]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 0.075
 temperature_value = temperature_function6
 water_phase = false
 [../]
 [./viscositysteam_8]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 0.05
 temperature_value = temperature_function6
 water_phase = false
 [../]
 [./viscositysteam_9]
 type = EOSViscosityFuncPPS
 water_steam_properties = water_steam_properties
 density_value = 0.025
 temperature_value = temperature_function7
 water_phase = false
 [../]
 []

[Executioner]
  type = Transient
  num_steps = 10
  dt = 1

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  nl_abs_tol = 1e-7
  end_time = 1000000
[]

[Outputs]
  output_initial = true
  csv = true
  exodus = false
  print_linear_residuals = true
  print_perf_log = true
[]
