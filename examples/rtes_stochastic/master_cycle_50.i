[StochasticTools]
[]
sampler = montecarlo
[Distributions]
  [depth]
    type = Uniform
    lower_bound = 1000
    upper_bound = 3000
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [injection_temp]
    type = Uniform
    lower_bound = ${fparse 100 + 273.15}
    upper_bound = ${fparse 300 + 273.15}
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [inj_ext_rate]  # need to adopt the aquifer thickness
    type = Uniform
    lower_bound = 0.5
    upper_bound = 2.5
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [distance_between_wells]
    type = Uniform
    lower_bound = 100
    upper_bound = 200
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [perm_aquifer]
    type = Uniform
    lower_bound = -15
    upper_bound = -11
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [porosity]
    type = Uniform
    lower_bound = 0.01
    upper_bound = 0.3
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [Tcond_aquifer]
    type = Uniform
    lower_bound = 2
    upper_bound = 4
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
[]

[Samplers]
  [montecarlo]
    type = MonteCarlo
    num_rows = 50000
    distributions = 'depth injection_temp inj_ext_rate distance_between_wells perm_aquifer porosity Tcond_aquifer'
    execute_on = 'PRE_MULTIAPP_SETUP'
  []
  [cartesian]
      type = CartesianProduct
      linear_space_items = '${Distributions/depth/cartprod}
                            ${Distributions/injection_temp/cartprod}
                            ${Distributions/inj_ext_rate/cartprod}
                            ${Distributions/distance_between_wells/cartprod}
                            ${Distributions/perm_aquifer/cartprod}
                            ${Distributions/porosity/cartprod}
                            ${Distributions/Tcond_aquifer/cartprod}'
      execute_on = 'PRE_MULTIAPP_SETUP'
  []
[]

[Controls]
  [cmdline]
    type = MultiAppCommandLineControl
    multi_app = runner
    sampler = ${sampler}
    param_names = 'depth injection_temp inj_ext_exponent distance_between_wells perm_exponent Materials/porosity_aquifer/porosity Tcond_aquifer'
  []
[]

[Transfers]
  [results]
    type = SamplerPostprocessorTransfer
    multi_app = runner
    sampler = ${sampler}
    to_vector_postprocessor = results
    from_postprocessor = 'recovery_rate total_recovery_time total_energy'
  []
[]

[VectorPostprocessors]
  [results]
    type = StochasticResults
  []
  [samples]
    type = SamplerData
    sampler = ${sampler}
  []
[]

[MultiApps]
  [runner]
    type = SamplerFullSolveMultiApp
    sampler = ${sampler}
    input_files = 'base_cycle_50.i'
    mode = batch-reset
    ignore_solve_not_converge = true
  []
[]

[Outputs]
  execute_on = 'INITIAL TIMESTEP_END'
  csv = true
[]
