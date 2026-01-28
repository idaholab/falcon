!include fracs_base.i

[MultiApps]
  [wells]
    type = TransientMultiApp
    app_type = FalconApp
    input_files = wells_sub.i
    max_procs_per_app = 1
    catch_up = true
    max_catch_up_steps = 10
    execute_on = 'TIMESTEP_END'
  []
[]

[Physics]
  [CoupledInjectionProduction]
    [inj_prod]
      injection_points = '${inj_point1} ${inj_point2} ${inj_point3}'
      production_points = '${pro_point1} ${pro_point2} ${pro_point3}'
      multi_app = wells
    []
  []
[]

[FunctorMaterials]
  [pp_change_fmat]
    type = ADFunctorChangeFunctorMaterial
    functor = porepressure
    change_over = fixed_point
    prop_name = pp_change
    take_absolute_value = true
  []
[]

[Postprocessors]
  [pp_max_change]
    type = ElementExtremeFunctorValue
    functor = pp_change
    execute_on = 'MULTIAPP_FIXED_POINT_CONVERGENCE'
  []
  [mass_rate_pro_total]
    type = ParsedPostprocessor
    expression = 'mass_rate_pro1 + mass_rate_pro2 + mass_rate_pro3'
    pp_names = 'mass_rate_pro1 mass_rate_pro2 mass_rate_pro3'
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [mass_rate_pro_total_derivative]
    type = ChangeOverTimePostprocessor
    postprocessor = mass_rate_pro_total
    divide_by_dt = true
    take_absolute_value = true
    execute_on = 'TIMESTEP_END'
  []
[]

[Convergence]
  [fp_conv]
    type = PostprocessorConvergence
    postprocessor = pp_max_change
    max_iterations = 10
    tolerance = 1e3
    verbose = true
  []
  [ss_conv]
    type = PostprocessorConvergence
    postprocessor = mass_rate_pro_total_derivative
    tolerance = 1e-5 # kg/s
    min_iterations = 1
    max_iterations = 10000000
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
  file_base = 'outputs/fracs_main/fracs'
[]
