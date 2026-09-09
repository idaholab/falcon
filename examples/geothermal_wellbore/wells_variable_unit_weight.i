# Production well using the NEW PorousFlowPeacemanBorehole behaviour: wellbore pressure built
# from a temperature-dependent fluid density (unit_weight_fp), integrated along the full well
# column using the in-well temperature at each point, instead of the single constant
# unit_weight in wells_constant_unit_weight.i.
#
# unit_weight_reference_pressure must stay above the local saturation pressure across the
# well's full temperature range (up to 503 K at the well bottom), or the fluid-density
# evaluation would fall onto the vapor branch. 8 MPa is comfortably in the liquid branch (and
# close to this well's actual in-well pressure) across the whole 303-503 K range.
#
# Otherwise identical to wells_constant_unit_weight.i - same well geometry, same DiracKernel
# names, same UserObjects/Postprocessors - so the two top-level input files are structurally
# identical except for this one file.

[UserObjects]
  [well_mass_outflow]
    type = PorousFlowSumQuantity
  []
  [well_heat_outflow]
    type = PorousFlowSumQuantity
  []
  [well_mass_point_flux]
    type = PorousFlowPointFluxQuantity
  []
  [well_heat_point_flux]
    type = PorousFlowPointFluxQuantity
  []
[]

[DiracKernels]
  [withdraw_fluid]
    type = PorousFlowPeacemanBorehole
    variable = porepressure
    point_file = geothermal_wellbore.bh
    function_of = pressure
    fluid_phase = 0
    character = well_character
    bottom_p_or_t = bottomhole_pressure
    unit_weight_fp = water
    unit_weight_temperature = temperature
    unit_weight_gravity = '0 -9.81 0'
    unit_weight_reference_pressure = 8E6
    use_mobility = true
    SumQuantityUO = well_mass_outflow
    PointFluxUO = well_mass_point_flux
  []
  [withdraw_heat]
    type = PorousFlowPeacemanBorehole
    variable = temperature
    point_file = geothermal_wellbore.bh
    function_of = pressure
    fluid_phase = 0
    character = well_character
    bottom_p_or_t = bottomhole_pressure
    unit_weight_fp = water
    unit_weight_temperature = temperature
    unit_weight_gravity = '0 -9.81 0'
    unit_weight_reference_pressure = 8E6
    use_mobility = true
    use_enthalpy = true
    SumQuantityUO = well_heat_outflow
    PointFluxUO = well_heat_point_flux
  []
[]

[Postprocessors]
  [well_mass_rate]
    type = PorousFlowPlotQuantity
    uo = well_mass_outflow
  []
  [well_heat_rate]
    type = PorousFlowPlotQuantity
    uo = well_heat_outflow
  []
[]

[VectorPostprocessors]
  [mass_point_flux]
    type = PorousFlowPlotPointFluxQuantity
    uo = well_mass_point_flux
  []
  [heat_point_flux]
    type = PorousFlowPlotPointFluxQuantity
    uo = well_heat_point_flux
  []
[]
