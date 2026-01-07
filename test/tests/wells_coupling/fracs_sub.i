!include fracs_base.i

[Physics]
  [CoupledInjectionProduction]
    [inj_prod]
      injection_points = '${inj_point1} ${inj_point2} ${inj_point3}'
      production_points = '${pro_point1} ${pro_point2} ${pro_point3}'
    []
  []
[]

[Outputs]
  file_base = 'outputs/wells_main/fracs'
[]
