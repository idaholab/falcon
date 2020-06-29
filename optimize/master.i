
[StochasticTools]
[]

[MultiApps]
  [runner]
    type = SamplerFullSolveMultiApp
    sampler = hypercube
    input_files = 'injection.i'
    mode = batch-restore
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
[]

[Samplers]
  [hypercube]
    type = LatinHypercube
    num_rows = 10
    distributions = 'injection_temp init_temp'
    num_bins = 10
  []
[]

[Transfers]
  [parameters]
    type = SamplerParameterTransfer
    multi_app = runner
    sampler = hypercube
    parameters = 'ICs/init_temp/value BCs/T_injection/value'
    to_control = 'stochastic'
  []
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
