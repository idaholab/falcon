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
