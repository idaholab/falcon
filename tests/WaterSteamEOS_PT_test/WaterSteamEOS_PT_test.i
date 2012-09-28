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
  active = 'temperature_function'
  [./pressure_function]
    type = ParsedFunction
    value = 1e6+1e4*t
  [../]
  [./temperature_function]
    type = ParsedFunction
    value = 10+10*t
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
  [./density]
    type = WaterSteamEOSDensityPPS_PT
    water_steam_properties = water_steam_properties
    pressure_value = 2e6
    temperature_value = temperature_function
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 20
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

