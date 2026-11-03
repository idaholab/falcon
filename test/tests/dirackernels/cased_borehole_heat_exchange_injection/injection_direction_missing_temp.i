# Minimal check that PorousFlowCasedBoreholeHeatExchange's flow_direction=injection mode marches
# in the opposite direction from the default (production) mode: fluid enters at the wellhead
# (index 0) at a prescribed injection_temperature, so the formation-to-well temperature
# difference (and hence the exchanged heat flux) should be LARGEST at the shallowest cased point
# and decay with depth - the mirror image of production, where it is largest at the deepest
# (open/cased-boundary-adjacent) point.
#
# gravity=0 and a uniform fixed bottom_p_or_t keep the mass-injection side simple (no hydrostatic
# column to reason about) - this test is about the cased kernel's own march direction, not about
# realistic well hydraulics.
[Mesh]
  coord_type = RZ
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 8
    xmin = 0
    xmax = 100
    ymin = -400
    ymax = 0
  []
[]

[GlobalParams]
  PorousFlowDictator = dictator
  gravity = '0 0 0'
[]

[Variables]
  [porepressure]
    initial_condition = 1e6
  []
  [temperature]
  []
[]

[ICs]
  [temperature_gradient]
    # 303.15 K at the surface (y=0), increasing with depth - distinct from the 250 K
    # injection_temperature at every point, so the exchanged flux is unambiguously nonzero and
    # its sign/magnitude pattern is unambiguous.
    type = FunctionIC
    variable = temperature
    function = geothermal_temperature
  []
[]

[Functions]
  [geothermal_temperature]
    type = ParsedFunction
    expression = '303.15 - 0.1*y'
  []
  [well_character]
    # Open interval: the bottom half of the well (y <= -200)
    type = ParsedFunction
    expression = 'if(y <= -200, 1, 0)'
  []
  [cased_character]
    # Cased section: the top half of the well (y > -200) - complementary to well_character
    type = ParsedFunction
    expression = 'if(y <= -200, 0, 1)'
  []
  [bottomhole_pressure]
    type = ParsedFunction
    expression = '2e6' # comfortably above the 1e6 IC, so the injector stays active throughout
  []
  [injection_temperature]
    type = ParsedFunction
    expression = '250' # well below the 303.15-343.15 K formation range, so the entry condition
                       # is unambiguous
  []
[]

[FluidProperties]
  [water]
    type = SimpleFluidProperties
    thermal_expansion = 0
  []
[]

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = porepressure
  temperature = temperature
  fp = water
  pressure_unit = Pa
[]

[Materials]
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1E-14 0 0  0 1E-14 0  0 0 1E-14'
  []
  [thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.5 0 0  0 2.5 0  0 0 2.5'
  []
  [rock_internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2700
    specific_heat_capacity = 800
  []
[]

[UserObjects]
  [well_mass_outflow]
    type = PorousFlowSumQuantity
  []
  [well_mass_point_flux]
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
  [open_interval_path]
    # The open interval: wellhead-side boundary (y=-200) to the well bottom (y=-400).
    type = PolylineDiracPoints
    points = '0 -200 0
              0 -400 0'
    weight = 0.1
  []
  [cased_path]
    # The cased section: wellhead (y=0) down to the open/cased boundary (y=-200).
    type = PolylineDiracPoints
    points = '0 0 0
              0 -200 0'
    weight = 0.1
  []
[]

[DiracKernels]
  [inject_fluid]
    type = PorousFlowPeacemanBorehole
    variable = porepressure
    x_coord_reporter = 'open_interval_path/x'
    y_coord_reporter = 'open_interval_path/y'
    z_coord_reporter = 'open_interval_path/z'
    weight_reporter = 'open_interval_path/weight'
    function_of = pressure
    fluid_phase = 0
    character = well_character
    bottom_p_or_t = bottomhole_pressure
    unit_weight = '0 0 0'
    use_mobility = true
    SumQuantityUO = well_mass_outflow
    PointFluxUO = well_mass_point_flux
  []
  [wellbore_heat_exchange]
    type = PorousFlowCasedBoreholeHeatExchange
    variable = temperature
    x_coord_reporter = 'cased_path/x'
    y_coord_reporter = 'cased_path/y'
    z_coord_reporter = 'cased_path/z'
    weight_reporter = 'cased_path/weight'
    character = cased_character
    heat_transfer_coefficient = 2.0
    flow_direction = injection
    mass_point_flux_vpp = mass_point_flux
    wellbore_fp = water
    wellbore_reference_pressure = 1e6
    SumQuantityUO = wellbore_heat_outflow
    PointFluxUO = wellbore_heat_point_flux
  []
[]

[VectorPostprocessors]
  [mass_point_flux]
    type = PorousFlowPlotPointFluxQuantity
    uo = well_mass_point_flux
  []
  [wellbore_heat_point_flux_out]
    type = PorousFlowPlotPointFluxQuantity
    uo = wellbore_heat_point_flux
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  dt = 1e4
  num_steps = 3
  nl_abs_tol = 1e-10
[]

[Outputs]
  file_base = injection_direction
  exodus = false
  csv = true
  execute_on = timestep_end
[]
