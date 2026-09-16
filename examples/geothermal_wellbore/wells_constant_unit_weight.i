# Production well using the OLD PorousFlowPeacemanBorehole behaviour: a single constant
# unit_weight. Representative density evaluated at the open interval's mean depth (1500m:
# T=453.15 K, P=1.429e7 Pa via the domain's water_weight), giving ~896.4 kg/m3, so
# unit_weight = -896.4 * 9.81 = -8794.0 Pa/m. This is a single-point estimate, not a true
# depth- or flow-weighted average over the interval (density actually ranges from ~940 kg/m3 at
# the top of the open interval to ~831 kg/m3 at the bottom) - see geothermal_wellbore.md for
# why that matters.
#
# Two PorousFlowPeacemanBorehole DiracKernels share the same well geometry and character
# function: one on porepressure (mass extraction), one on temperature (use_enthalpy=true, heat
# extraction) - the correct/only mechanism for well-to-formation heat exchange in this module,
# since it is advective only (the extracted fluid carries its own enthalpy).

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
    unit_weight = '0 -8794.0 0'
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
    unit_weight = '0 -8794.0 0'
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
