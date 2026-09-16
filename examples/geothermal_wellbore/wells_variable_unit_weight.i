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
  [wellbore_heat_outflow]
    type = PorousFlowSumQuantity
  []
  [wellbore_heat_point_flux]
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
  [wellbore_heat_exchange]
    # Conductive heat exchange between the cased section (above the open interval) and the
    # surrounding cap, complementing withdraw_heat's advective-only exchange below - see
    # PorousFlowCasedBoreholeHeatExchange.md.
    #
    # heat_transfer_coefficient = 1.5 W/m^2/K is a realistic casing-side resistance estimate
    # (in-well film + steel casing + cement sheath). At this cap permeability (see
    # model_common.i - realistic, unlike an earlier revision's deliberately very low value),
    # thermal pressurization is no longer a binding constraint the way it was before, but a
    # separate numerical wall remains around day 156 of the run above roughly h=1.5-1.8
    # (confirmed by bisection; unrelated to thermal pressurization - raising nl_max_its does not
    # rescue h=1.8, which fails via DIVERGED_LINE_SEARCH even at dt=dtmin). 1.5 stays safely
    # below that wall.
    type = PorousFlowCasedBoreholeHeatExchange
    variable = temperature
    point_file = geothermal_wellbore.bh
    character = cased_character
    heat_transfer_coefficient = 1.5
    mass_point_flux_vpp = mass_point_flux
    wellbore_fp = water
    wellbore_reference_pressure = 8E6
    SumQuantityUO = wellbore_heat_outflow
    PointFluxUO = wellbore_heat_point_flux
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
  [wellbore_heat_rate]
    type = PorousFlowPlotQuantity
    uo = wellbore_heat_outflow
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
  [wellbore_heat_point_flux_out]
    type = PorousFlowPlotPointFluxQuantity
    uo = wellbore_heat_point_flux
  []
[]
