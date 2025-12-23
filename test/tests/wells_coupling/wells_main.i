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

[Convergence]
  [fp_conv]
    type = DefaultMultiAppFixedPointConvergence
    fixed_point_max_its = 10
    fixed_point_abs_tol = 1.0
    verbose = true
  []
[]

[Executioner]
  multiapp_fixed_point_convergence = fp_conv
[]

[Outputs]
  file_base = 'outputs/wells_main/wells'
[]
