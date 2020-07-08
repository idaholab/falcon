
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

well_length = 10 # need to change for the 5 meshes with different aquifer thickness

[Distributions]
  [init_temp]
    type = Uniform
    lower_bound = ${fparse 20 + 273.15}
    upper_bound = ${fparse 80 + 273.15}
  []
  [init_pp]
    type = Uniform
    lower_bound = 1E7
    upper_bound = 4E7
  []
  [injection_temp]
    type = Uniform
    lower_bound = ${fparse 100 + 273.15}
    upper_bound = ${fparse 300 + 273.15}
  []
  [inj_ext_rate]  # need to adopt the aquifer thickness
    type = Uniform
    lower_bound = ${fparse 5 / well_length / 2} 
    upper_bound = ${fparse 50 / well_length / 2}
  []
  [distance_between_wells]
    type = Uniform
    lower_bound = 10
    upper_bound = 200
  []
  [perm_aquifer]
    type = Uniform
    lower_bound = 1e-17
    upper_bound = 1e-12
  []
  [porosity]
    type = Uniform
    lower_bound = 0.01
    upper_bound = 0.3
  [Tcond_aquifer]
    type = Uniform
    lower_bound = 2
    upper_bound = 3
  []
  [switch_extraction]
    type = Uniform
    lower_bound = 7000
    upper_bound = 15000
  []
[]

[Samplers]
  [hypercube]
    type = LatinHypercube
    num_rows = 15
    distributions = 'init_temp init_pp injection_temp inj_ext_rate distance_between_wells perm_aquifer porosity Tcond_aquifer switch_extraction'
    num_bins = 4
    execute_on = 'PRE_MULTIAPP_SETUP'
  []
[]

[Controls]
  [cmdline]
    type = MultiAppCommandLineControl
    multi_app = runner
    sampler = hypercube
    param_names = 'T_ini_bc pp_ini_bc DiracKernels/injection_T/T_in/value inj_ext_flux distance_between_wells perm_aquifer Materials/porosity_aquifer/porosity Tcond_aquifer switch_to_extraction'
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

