
[StochasticTools]
[]

[MultiApps]
  [runner]
    type = SamplerFullSolveMultiApp
    sampler = hypercube
    input_files = 'inject-extract.i'
    mode = batch-reset
  []
[]

[Distributions]
  [injection_temp]
    type = Uniform
    lower_bound = ${fparse 100 + 273}
    upper_bound = ${fparse 300 + 273}
  []
  [init_temp]
    type = Uniform
    lower_bound = ${fparse 20 + 273}
    upper_bound = ${fparse 80 + 273}
  []
  [switch_extraction]
    type = Uniform
    lower_bound = 7000
    upper_bound = 15000
  []
  [distance_between_wells]
    type = Uniform
    lower_bound = 10
    upper_bound = 20
  []
[]

[Samplers]
  [hypercube]
    type = LatinHypercube
    num_rows = 15
    distributions = 'injection_temp init_temp switch_extraction distance_between_wells'
    num_bins = 4
    execute_on = 'PRE_MULTIAPP_SETUP'
  []
[]

[Controls]
  [cmdline]
    type = MultiAppCommandLineControl
    multi_app = runner
    sampler = hypercube
    param_names = 'ICs/init_temp/value BCs/T_injection/value switch_to_extraction distance_between_wells'
  []
[]

[Transfers]
  [results]
    type = SamplerPostprocessorTransfer
    multi_app = runner
    sampler = hypercube
    to_vector_postprocessor = results
    from_postprocessor = 'outlet_mass_kg outlet_enthalpy_J'
  []
[]

[VectorPostprocessors]
  [results]
    type = StochasticResults
  []
  [samples]
    type = SamplerData
    sampler = hypercube
  []
  [stats]
    type = Statistics
    vectorpostprocessors = results
    compute = 'mean'
    ci_method = 'percentile'
    ci_levels = '0.05'
  []
[]

[Outputs]
  execute_on = 'INITIAL TIMESTEP_END'
  csv = true
[]

