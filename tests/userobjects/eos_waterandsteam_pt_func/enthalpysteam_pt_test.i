# This test is used to verify the enthalpy_steam output of the waterAndSteamEquationOfStatePT 
# function in the WaterSteamEOS UserObject.  A combination of incremented pressures and 
# temperatures are inputed into the EOSWaterAndSteamPTFuncPPS and the steam enthalpy is outputted.  
# The results are compared to a csv file which contains IAWPS97 water and steam formulation data
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
 value = 100+20*t 
 [../]
 
 [./temperature_function2]
 type = ParsedFunction
 value = 120+20*t 
 [../]
 
 [./temperature_function3]
 type = ParsedFunction
 value = 140+20*t 
 [../]
 
 [./temperature_function4]
 type = ParsedFunction
 value = 150+20*t 
 [../]
 
 [./temperature_function5]
 type = ParsedFunction
 value = 160+20*t 
 [../]
 
 [./temperature_function6]
 type = ParsedFunction
 value = 170+20*t 
 [../]
 
 [./temperature_function7]
 type = ParsedFunction
 value = 180+20*t 
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
 [./enthalpy_100KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.1e6
 temperature_value = temperature_function1
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_200KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.2e6
 temperature_value = temperature_function2
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_300KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.3e6
 temperature_value = temperature_function3
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_400KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.4e6
 temperature_value = temperature_function4
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_500KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.5e6
 temperature_value = temperature_function5
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_600KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.6e6
 temperature_value = temperature_function5
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_700KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.7e6
 temperature_value = temperature_function6
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_800KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.8e6
 temperature_value = temperature_function7
 water_phase = false
 test_density = false
 [../]
 
 [./enthalpy_900KPa]
 type = EOSWaterAndSteamPTFuncPPS
 water_steam_properties = water_steam_properties
 pressure_value = 0.9e6
 temperature_value = temperature_function7
 water_phase = false
 test_density = false
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
  nl_abs_tol = 1e-5
  end_time = 1000000
[]

[Output]
  output_initial = true
  postprocessor_csv = true
  exodus = false
  perf_log = true
[]

