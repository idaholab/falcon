# Rate-controlled geothermal injection well: instead of prescribing a bottomhole pressure and
# letting mass flow emerge from it (as the production wells do), this well is given a target mass
# rate, and PorousFlowRateControlledBoreholePressure works out what bottomhole pressure would
# deliver it. See geothermal_wellbore.md's "Rate-controlled injection" section for the governing
# relationships.
#
# Shares model_common.i and injection_zones.i's mesh/materials with the production wells - same
# well geometry (open over the bottom 1000m, cased above), same geothermal_wellbore.bh point
# file for the open interval, same cased_character/heat_transfer_coefficient - only the driving
# mechanism (rate vs. pressure) and the fluid's own direction of travel differ.

[Functions]
  [injection_character]
    # Mirror of model_common.i's well_character, but NEGATIVE: PorousFlowPeacemanBorehole acts
    # as a source (injector) only where character < 0 and porepressure < wellbore pressure.
    type = ParsedFunction
    expression = 'if(y <= -1000, -1, 0)'
  []
  [injection_temperature]
    # 40 degC reinjection - a representative geothermal spent-brine temperature, well below the
    # 130-230 degC (403-503 K) formation temperature over the open interval.
    type = ParsedFunction
    expression = '313.15'
  []
  [target_injection_rate]
    # NEGATIVE, in kg/s, for INJECTION - this is not a typo. PorousFlow's DiracKernel sinks
    # report "outflow": flow FROM the porespace OUT of the system. Production (character > 0)
    # removes mass from the reservoir, so outflow > 0. Injection (character < 0, as this well's
    # own injection_character is) ADDS mass to the reservoir - outflow FROM the reservoir is
    # therefore NEGATIVE, exactly the same sign convention model_common.i's own well_character/
    # cased_character already use throughout this example (character < 0 => injector). This
    # target_rate is compared directly against rate_postprocessor below, which reports that same
    # signed quantity - so both must use the same sign, and it must be PorousFlow's own.
    #
    # Calibrated against a fixed-bottom_p_or_t sweep of this same model at t=0 (see the example's
    # own verification notes): -20 kg/s converges cleanly (monotonically, no oscillation) within
    # bhp_control's pressure clamps below over the 5-year run; -40 pins the controller against
    # max_pressure without ever reaching the target, since the near-well reservoir pressure
    # itself rises enough over the run to erode most of the available overpressure.
    type = ParsedFunction
    expression = '-20'
  []
  [bottomhole_pressure_control]
    # Feeds the controller's corrected pressure into bottom_p_or_t, which is typed FunctionName.
    type = FalconPostprocessorFunction
    pp = bhp_control
  []
[]

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

[Reporters]
  [cased_path]
    # Same convention as the production wells' own cased_path (see PolylineDiracPoints.md):
    # auto-placed points at the mesh's own 50m resolution, wellhead down to the cased/open
    # boundary at y=-1000, which must match geothermal_wellbore.bh's own shallowest point.
    type = PolylineDiracPoints
    points = '0 0 0
              0 -1000 0'
    weight = 0.1
  []
[]

[DiracKernels]
  [inject_fluid]
    type = PorousFlowPeacemanBorehole
    variable = porepressure
    point_file = geothermal_wellbore.bh
    function_of = pressure
    fluid_phase = 0
    character = injection_character
    bottom_p_or_t = bottomhole_pressure_control
    # A CONSTANT unit_weight, deliberately not unit_weight_fp: that option builds the wellbore
    # pressure column from unit_weight_temperature sampled at each well point, ie the FORMATION
    # temperature - a reasonable proxy for a producer's own in-well fluid, but wrong for an
    # injector, whose column is cold injected water all the way down. 9810 N/m^3 matches water
    # near the 313.15 K injection_temperature.
    unit_weight = '0 -9810 0'
    use_mobility = true
    SumQuantityUO = well_mass_outflow
    PointFluxUO = well_mass_point_flux
  []
  [inject_heat]
    type = PorousFlowPeacemanEnthalpySink
    variable = temperature
    point_file = geothermal_wellbore.bh
    function_of = pressure
    fluid_phase = 0
    character = injection_character
    bottom_p_or_t = bottomhole_pressure_control
    unit_weight = '0 -9810 0'
    use_mobility = true
    fp = water
    pressure = porepressure
    injection_temperature = injection_temperature
    SumQuantityUO = well_heat_outflow
    PointFluxUO = well_heat_point_flux
  []
  [wellbore_heat_exchange]
    # Conductive heat exchange between the cased section (above the open interval) and the
    # surrounding cap - see PorousFlowCasedBoreholeHeatExchange.md's "Flow direction" section.
    # injection_temperature is deliberately the SAME Function as inject_heat's own: the raw
    # wellhead temperature, not fed back from this kernel's own computed arrival temperature at
    # the open/cased boundary (a documented simplification - see geothermal_wellbore.md).
    type = PorousFlowCasedBoreholeHeatExchange
    variable = temperature
    x_coord_reporter = 'cased_path/x'
    y_coord_reporter = 'cased_path/y'
    z_coord_reporter = 'cased_path/z'
    weight_reporter = 'cased_path/weight'
    character = cased_character
    heat_transfer_coefficient = 1.5
    flow_direction = injection
    injection_temperature = injection_temperature
    mass_point_flux_vpp = mass_point_flux
    wellbore_fp = water
    wellbore_reference_pressure = 8E6
    SumQuantityUO = wellbore_heat_outflow
    PointFluxUO = wellbore_heat_point_flux
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

[Postprocessors]
  [injected_mass_rate]
    # A true rate (kg/s) - PorousFlowPointFluxQuantity (behind mass_point_flux) is NOT multiplied
    # by dt, unlike PorousFlowSumQuantity/PorousFlowPlotQuantity (well_mass_rate below).
    type = VectorPostprocessorReductionValue
    vectorpostprocessor = mass_point_flux
    vector_name = flux
    value_type = sum
  []
  [bhp_control]
    type = PorousFlowRateControlledBoreholePressure
    rate_postprocessor = injected_mass_rate
    target_rate = target_injection_rate
    # Calibrated directly against the model's own t=0 state (a short fixed-bottom_p_or_t sweep -
    # see the example's own verification notes): 2.18e7 Pa delivers -20.04 kg/s at t=0, matching
    # the target almost exactly. Starting this close means the controller has essentially nothing
    # to correct from step 1 onward, rather than spending many timesteps - and, since
    # IterationAdaptiveDT grows dt aggressively (up to 1 year), potentially a large fraction of
    # simulated TIME - closing a big initial gap. A cruder initial guess (2.1e7, tried first) put
    # the controller ~9 kg/s off target for the first ~50 days, then needed until ~3.5 years (not
    # a handful of timesteps) to fully close the remaining gap, one dt-growing step at a time.
    initial_pressure = 2.18e7
    initial_pressure_step = 5e4
    # 1e6, not the more conservative 5e5 tried first: the well's real response is not exactly
    # affine over the ~7e6 Pa range this run ends up covering (near-well mobility evolves as the
    # cold injectate advances and reservoir pressure itself rises), so a secant slope measured
    # near one operating point under-estimates the correction needed once conditions have moved
    # far enough - PorousFlowRateControlledBoreholePressure's min_relative_error_for_floor
    # mechanism compensates for this (see its own docs), but its escalated step is sized
    # relative to max_pressure_change, so 5e5 left the rate needing until ~2 years to fully
    # settle. 1e6 settles within about 3-4 weeks and then tracks the target closely (within a
    # few percent) for the rest of the run - the direction-consistency check in the same
    # mechanism keeps this from overshooting into a persistent oscillation.
    max_pressure_change = 1e6
    min_pressure = 1.902e7 # the initial hydrostatic pressure at the well bottom - the controller
                           # can never wander into the production regime
    max_pressure = 4.0e7   # a generous ceiling - never approached once calibrated (see above);
                           # a rough fracture-gradient estimate at this depth would allow far more
  []
  [wellbore_heat_rate]
    type = PorousFlowPlotQuantity
    uo = wellbore_heat_outflow
  []
  [well_mass_rate]
    type = PorousFlowPlotQuantity
    uo = well_mass_outflow
  []
  [well_heat_rate]
    type = PorousFlowPlotQuantity
    uo = well_heat_outflow
  []
[]
