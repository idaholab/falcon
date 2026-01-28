
[Functions]
  [insitu_T]
    type = ParsedFunction
    expression = '426.67 - 0.0733333*(z-1150)'
  []
  [initial_p_fn]
    type = HydrostaticPressureFunction
    reference_pressure = ${units 1 atm -> Pa}
    reference_temperature = 305.670055
    reference_point = '0 0 ${z_surface}'
    fluid_properties = fp_water
    gravity_vector = ${gravity_vector}
  []
[]
