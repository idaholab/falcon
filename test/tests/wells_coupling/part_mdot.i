[Functions]
  [inlet_mdot_fn]
    type = TimeRampFunction
    initial_value = 0
    final_value = ${fparse 10 * 2.65 * total_mass_ratio} # 10 barrels/min to kg/s, then reduced by fraction
    initial_time = 0
    ramp_duration = ${injection_ramp_duration}
  []
[]
