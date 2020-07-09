
[StochasticTools]
[]

sampler = montecarlo

well_length = 10 # need to change for the 5 meshes with different aquifer thickness

[Distributions]
  [init_temp]
    type = Uniform
    lower_bound = ${fparse 20 + 273.15}
    upper_bound = ${fparse 80 + 273.15}
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [init_pp]
    type = Uniform
    lower_bound = 1E7
    upper_bound = 4E7
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
    lower_bound = ${fparse 5 / well_length / 2} 
    upper_bound = ${fparse 50 / well_length / 2}
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [distance_between_wells]
    type = Uniform
    lower_bound = 10
    upper_bound = 200
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
  [perm_aquifer]
    type = Uniform
    lower_bound = 1e-17
    upper_bound = 1e-12
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
    upper_bound = 3
    n = 2
    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
  []
#  [switch_extraction]
#    type = Uniform
#    #lower_bound = 7000
#    #upper_bound = 15000
#    lower_bound = 100
#    upper_bound = 200
#    n = 2
#    cartprod = '${lower_bound} ${fparse (upper_bound - lower_bound) / (n - 1)} ${n}'
#  []
[]

[Samplers]
  [montecarlo]
    type = MonteCarlo
    num_rows = 96
    distributions = 'init_temp init_pp injection_temp inj_ext_rate distance_between_wells perm_aquifer porosity Tcond_aquifer'
    execute_on = 'PRE_MULTIAPP_SETUP'
  []
  [cartesian]
      type = CartesianProduct
      linear_space_items = '${Distributions/init_temp/cartprod}
                            ${Distributions/init_pp/cartprod}
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
    param_names = 'T_ini_bc pp_ini_bc DiracKernels/injection_T/T_in inj_ext_flux distance_between_wells perm_aquifer Materials/porosity_aquifer/porosity Tcond_aquifer'
  []
[]

[Transfers]
  [results]
    type = SamplerPostprocessorTransfer
    multi_app = runner
    sampler = ${sampler}
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
    sampler = ${sampler}
  []
  [stats]
    type = Statistics
    vectorpostprocessors = results
    compute = 'mean'
    ci_method = 'percentile'
    ci_levels = '0.05'
  []
[]

[MultiApps]
  [runner]
    type = SamplerFullSolveMultiApp
    sampler = ${sampler}
    input_files = 'inject-extract.i'
    mode = batch-reset
  []
[]

[Outputs]
  execute_on = 'INITIAL TIMESTEP_END'
  csv = true
[]

