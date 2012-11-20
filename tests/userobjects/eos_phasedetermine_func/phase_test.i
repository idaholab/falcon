# This test is used to verify the output of the phaseDetermine function in the
# WaterSteamEOS UserObject.  A combination of incremented pressures and 
# enthalpies are inputed into the EOSPhaseDetermineFuncPPS and the phase is
# outputted.  The results are compared to a csv file which contains IAWPS97 
# water and steam formulation data
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
  [./pressure_function]
    type = ParsedFunction
    value = 0.1e6+0.6e6*t
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
  [./enthalpy_1]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 1e4
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_2]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 2.5e4
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_3]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 5e4
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_4]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 7.5e4
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_5]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 1e5
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_6]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 2.5e5
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_7]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 5e5
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_8]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 7.5e5
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_9]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 1e6
    pressure_value = pressure_function
    test_phase = true
  [../]
  [./enthalpy_10]
    type = EOSPhaseDetermineFuncPPS
    water_steam_properties = water_steam_properties
    enthalpy_value = 2.5e6
    pressure_value = pressure_function
    test_phase = true
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 40
  dt = 1
  petsc_options = -snes_mf_operator
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

