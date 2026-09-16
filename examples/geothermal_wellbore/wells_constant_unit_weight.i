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
    # heat_transfer_coefficient is capped well below a "casing resistance only" physical
    # estimate (~10) by this model's own cap properties: the cap's permeability is deliberately
    # very low (to keep the well's pressure influence confined - see model_common.i), so heat
    # injected there cannot relieve via flow and instead thermally pressurizes the pore fluid.
    # Above roughly h=0.1 that pressurization runs away and the solve diverges within the first
    # simulated day, regardless of how small a fraction of the advected heat rate it represents
    # (confirmed by bisection: h=0.01 runs stably for the full 5 years, h=0.1 does not - both
    # well below the point where the effect is dominated by anything resembling real casing
    # resistance). 0.01 keeps the model numerically stable; the resulting effect is small
    # (~0.08% of well_heat_rate) but real.
    type = PorousFlowCasedBoreholeHeatExchange
    variable = temperature
    point_file = geothermal_wellbore.bh
    character = cased_character
    heat_transfer_coefficient = 0.01
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
