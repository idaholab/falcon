!include params_base.i
!include params_mdot.i

!include part_base.i
!include part_ics.i
!include part_mdot.i

n_elems_inj1 = 100
n_elems_inj2 = 100
n_elems_inj3 = 100
n_elems_pro1 = 100
n_elems_pro2 = 100
n_elems_pro3 = 100

radius = 0.09
A_inj = ${fparse pi * radius^2}
A_pro = ${A_inj}

inj_junction_volume = ${fparse A_inj^(3/2)} # cube with A_inj side area
pro_junction_volume = ${fparse A_pro^(3/2)} # cube with A_pro side area

inj_surface_point = '${x1} ${y1} ${z_surface}'
pro_surface_point = '${x1} ${y1} ${z_surface}'

A_inj1 = ${A_inj}
A_inj2 = ${A_inj}
A_inj3 = ${A_inj}

A_pro1 = ${A_pro}
A_pro2 = ${A_pro}
A_pro3 = ${A_pro}

[ActionComponents]
  [inj]
    type = InjectionWell
    surface_point = ${inj_surface_point}
    junction_points = '${inj_point1} ${inj_point2} ${inj_point3}'
    section_n_elems = '${n_elems_inj1} ${n_elems_inj2} ${n_elems_inj3}'
    area = ${A_inj}
    junction_coupling_areas = '${A_inj1} ${A_inj2} ${A_inj3}'
    fracture_direction = '1 0 0'
    junction_volume = ${inj_junction_volume}
    inlet_mass_flow_rate = inlet_mdot_fn
    inlet_temperature = insitu_T
    initial_pressure = initial_p_fn
    initial_temperature = insitu_T
    fluid_properties = fp_water
  []
  [pro]
    type = ProductionWell
    surface_point = ${pro_surface_point}
    junction_points = '${pro_point1} ${pro_point2} ${pro_point3}'
    section_n_elems = '${n_elems_pro1} ${n_elems_pro2} ${n_elems_pro3}'
    area = ${A_pro}
    junction_coupling_areas = '${A_pro1} ${A_pro2} ${A_pro3}'
    fracture_direction = '-1 0 0'
    junction_volume = ${pro_junction_volume}
    outlet_pressure = initial_p_fn
    initial_pressure = initial_p_fn
    initial_temperature = insitu_T
    fluid_properties = fp_water
  []
[]

# This is necessary to trigger some THM actions
[Components]
[]

[Postprocessors]
  [mass_rate_inlet]
    type = ADFlowBoundaryFlux1Phase
    boundary = inj_inlet
    equation = mass
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [p_inlet]
    type = SideAverageValue
    boundary = inj_inlet
    variable = p
    execute_on = 'INITIAL TIMESTEP_END'
  []

  [mass_rate_outlet]
    type = ADFlowBoundaryFlux1Phase
    boundary = pro_outlet
    equation = mass
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [p_outlet]
    type = SideAverageValue
    boundary = pro_outlet
    variable = p
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient
  scheme = bdf2

  start_time = ${initial_time}
  end_time = ${final_time}

  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 10
    optimal_iterations = 5
    iteration_window = 0
    growth_factor = 1.5
    cutback_factor = 0.8
  []
  dtmin = 1e-4

  solve_type = NEWTON
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-6
  nl_max_its = 10

  l_tol = 1e-3
  l_max_its = 10

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Outputs]
  exodus = true
  [xml]
    type = XMLOutput
    execute_vector_postprocessors_on = 'INITIAL TIMESTEP_END'
  []
  [csv]
    type = CSV
    execute_vector_postprocessors_on = 'NONE'
  []
  [console]
    type = Console
    execute_postprocessors_on = 'NONE'
  []
  print_linear_residuals = false
[]
