!include wells_base.i

[ActionComponents]
  [inj]
    multi_app = fracs
  []
  [pro]
    multi_app = fracs
  []
[]

[MultiApps]
  [fracs]
    type = TransientMultiApp
    app_type = FalconApp
    input_files = fracs_sub.i
    execute_on = 'TIMESTEP_END'
  []
[]

[Postprocessors]
  [mass_rate_outlet_derivative]
    type = ChangeOverTimePostprocessor
    postprocessor = mass_rate_outlet
    divide_by_dt = true
    take_absolute_value = true
    execute_on = 'TIMESTEP_END'
  []
[]

[Convergence]
  [fp_conv]
    type = DefaultMultiAppFixedPointConvergence
    fixed_point_max_its = 10
    fixed_point_abs_tol = 1.0
    verbose = true
  []
  [ss_conv]
    type = PostprocessorConvergence
    postprocessor = mass_rate_outlet_derivative
    tolerance = 1e-6 # kg/s
    min_iterations = 1
    max_iterations = 1e7
    verbose = true
  []
[]

[Executioner]
  multiapp_fixed_point_convergence = fp_conv
  steady_state_detection = true
  steady_state_start_time = ${injection_ramp_duration}
  steady_state_convergence = ss_conv
[]

[Outputs]
  file_base = 'outputs/wells_main/wells'
[]
