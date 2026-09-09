# Shared model for the geothermal wellbore example: axisymmetric (RZ) mesh, two-layer
# formation, geothermal field, and fluid properties. Everything except the well itself lives
# here; !include'd by both top-level input files, each of which pulls in a different
# well-definition file (production of an old constant unit_weight, vs. the new
# temperature-dependent unit_weight_fp), so the two variants are identical everywhere except
# the well.
#
# Coordinates: x = r (radial, 0 at the well), y = depth (land surface at y=0, domain bottom at
# y=-2000, negative downward). coord_type=RZ defaults to rotation about the y-axis, so x is
# the radial coordinate.
#
# Geometry: r from 0 to 1000m (graded mesh, fine near the well), y from 0 to -2000m. The top
# 1000m is a low-permeability cap; the rest is the reservoir. This split coincides with the
# well's cased/open boundary (see well_character below): casing runs entirely through the cap,
# the open completion entirely in the reservoir - matching conventional well design. The well
# is open to the formation over its bottom 1000m (y=-1000 to -2000), spanning an in-well
# temperature range of 403-503 K.
#
# The cap's thermal properties are deliberately exaggerated (real caprock would take
# millennia to show any measurable thermal response over this depth) so that the well's
# interaction with the surrounding formation - the point of this example - is visible on a
# runnable timescale. Permeability in both layers is likewise well below realistic geologic
# values, to keep that thermal response spatially confined to the region around the well
# rather than spread across the whole domain by fast pressure diffusion. Both are idealizations
# made for clarity, not a claim about real reservoir/caprock properties - see
# geothermal_wellbore.md for the reasoning.

[Mesh]
  coord_type = RZ
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 40
    ny = 40
    xmin = 0
    xmax = 1000
    ymin = -2000
    ymax = 0
    bias_x = 1.1 # radial grading: small elements near the well (r=0), growing outward
  []
  [cap_block]
    type = SubdomainBoundingBoxGenerator
    input = gen
    bottom_left = '0 -1000 -1'
    top_right = '1000 0 1'
    block_id = 1
  []
  [rename]
    type = RenameBlockGenerator
    input = cap_block
    old_block = '0 1'
    new_block = 'reservoir cap'
  []
[]

[GlobalParams]
  PorousFlowDictator = dictator
  gravity = '0 -9.81 0' # acts along 'y' (depth) in this RZ mesh, not 'z' (always 0 here)
[]

[Variables]
  [porepressure]
  []
  [temperature]
    scaling = 1e-6
  []
[]

[Functions]
  [geothermal_temperature]
    # 30 degC (303.15 K) at the surface, 100 degC/km (0.1 K/m) gradient, hotter with depth.
    type = ParsedFunction
    symbol_names = 'T_surface gradient'
    symbol_values = '303.15 0.1'
    expression = 'T_surface - gradient * y'
  []
  [hydrostatic_pressure]
    # Linear hydrostatic profile using a single representative domain-average water unit
    # weight, independent of the well's own, separately-derived unit_weight in the
    # well-definition files.
    type = ParsedFunction
    symbol_names = 'P_surface water_weight'
    symbol_values = '1e5 9459.84'
    expression = 'P_surface - water_weight * y'
  []
  [bottomhole_pressure]
    # Fixed-BHP producer: hydrostatic profile minus a constant drawdown, evaluated at the well
    # bottom (y=-2000) by PorousFlowPeacemanBorehole.
    type = ParsedFunction
    symbol_names = 'P_surface water_weight drawdown'
    symbol_values = '1e5 9459.84 1e6'
    expression = 'P_surface - water_weight * y - drawdown'
  []
  [well_character]
    # Open (character=1) only over the bottom 1000m of the well (y <= -1000); cased
    # (character=0) above that. All 21 points in geothermal_wellbore.bh (0 to -2000) stay in
    # point_file regardless, so the unit_weight_fp variant integrates the density profile over
    # the full column.
    type = ParsedFunction
    expression = 'if(y <= -1000, 1, 0)'
  []
[]

[ICs]
  [porepressure]
    type = FunctionIC
    variable = porepressure
    function = hydrostatic_pressure
  []
  [temperature]
    type = FunctionIC
    variable = temperature
    function = geothermal_temperature
  []
[]

[BCs]
  # Single far-field (outer-radius) recharge boundary, replacing what the well extracts. No BC
  # is needed at r=0 (the symmetry axis - handled automatically by the RZ Jacobian factors).
  [porepressure_recharge]
    type = FunctionDirichletBC
    variable = porepressure
    boundary = 'right'
    function = hydrostatic_pressure
  []
  [temperature_recharge]
    type = FunctionDirichletBC
    variable = temperature
    boundary = 'right'
    function = geothermal_temperature
  []
[]

[FluidProperties]
  [water]
    type = Water97FluidProperties
  []
[]

[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = porepressure
  temperature = temperature
  fp = water
  pressure_unit = Pa
  stabilization = full
[]

[Materials]
  [porosity_cap]
    type = PorousFlowPorosityConst
    porosity = 0.05
    block = cap
  []
  [porosity_reservoir]
    type = PorousFlowPorosityConst
    porosity = 0.1
    block = reservoir
  []
  # Permeability in both layers is lowered well below typical geologic values (reservoir would
  # normally be ~1E-14 m^2, cap ~1E-16 m^2) to keep the well's pressure/thermal influence
  # confined to roughly 100-300m around the well over the run - both layers need lowering
  # together, since leaving either one relatively higher lets its faster diffusion smear the
  # other's localized signal back out laterally. See geothermal_wellbore.md for the derivation.
  [permeability_cap]
    type = PorousFlowPermeabilityConst
    permeability = '1E-19 0 0  0 1E-19 0  0 0 1E-19'
    block = cap
  []
  [permeability_reservoir]
    type = PorousFlowPermeabilityConst
    permeability = '1E-15 0 0  0 1E-15 0  0 0 1E-15'
    block = reservoir
  []
  # Cap thermal properties are exaggerated (vs. real caprock's ~2.5 W/m/K, ~900 J/kg/K) so the
  # well's thermal interaction with the formation is visible on a runnable timescale.
  # Conductivity is anisotropic - xx=radial, yy=vertical/axial, zz=unused (out-of-plane,
  # irrelevant for a 2D RZ mesh) - vertical stays higher (faster transport toward the surface)
  # while radial stays closer to realistic, so the thermal halo doesn't smear outward as fast
  # as it rises.
  [thermal_conductivity_cap]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2 0 0  0 10 0  0 0 2'
    block = cap
  []
  [rock_internal_energy_cap]
    type = PorousFlowMatrixInternalEnergy
    density = 2700
    specific_heat_capacity = 80
    block = cap
  []
  # Reservoir thermal properties stay close to realistic geologic media - only the cap above
  # is meant to have an exaggerated response.
  [thermal_conductivity_reservoir]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.5 0 0 0 2.5 0 0 0 2.5'
    block = reservoir
  []
  [rock_internal_energy_reservoir]
    type = PorousFlowMatrixInternalEnergy
    density = 2700
    specific_heat_capacity = 800
    block = reservoir
  []
[]

[Postprocessors]
  # Points are (x, y, z) = (r, depth, 0) in this RZ mesh - NOT (x, y, z) = (0, 0, depth).
  [pp_wellhead]
    type = PointValue
    variable = porepressure
    point = '0 0 0'
  []
  [pp_bottomhole_formation]
    type = PointValue
    variable = porepressure
    point = '0 -2000 0'
  []
  [T_wellhead]
    type = PointValue
    variable = temperature
    point = '0 0 0'
  []
  [T_bottomhole_formation]
    type = PointValue
    variable = temperature
    point = '0 -2000 0'
  []
  [T_open_top]
    # Formation temperature at y=-1000, the shallowest open (perforated) point - the last point
    # along the upward flow path where fluid still enters the well before the cased section.
    type = PointValue
    variable = temperature
    point = '0 -1000 0'
  []
  # Temperature vs. time at a series of depths along the well axis (r=0), spanning both the
  # cap (where the well's thermal interaction with the formation shows up as a halo) and the
  # open interval (where extraction directly cools the formation). Together with T_wellhead
  # (y=0), T_open_top (y=-1000) and T_bottomhole_formation (y=-2000) above, this gives depths
  # 0, -200, -400, -600, -800, -1000, -1500, -2000.
  [T_y200]
    type = PointValue
    variable = temperature
    point = '0 -200 0'
  []
  [T_y400]
    type = PointValue
    variable = temperature
    point = '0 -400 0'
  []
  [T_y600]
    type = PointValue
    variable = temperature
    point = '0 -600 0'
  []
  [T_y800]
    type = PointValue
    variable = temperature
    point = '0 -800 0'
  []
  [T_y1500]
    type = PointValue
    variable = temperature
    point = '0 -1500 0'
  []
[]

[VectorPostprocessors]
  # Temperature vs. radius at the cap/reservoir interface (y=-1000, right at the well's open
  # interval) - the primary "well interaction" visual: a real, well-centered halo forms here.
  [temperature_halo]
    type = LineValueSampler
    variable = temperature
    start_point = '0 -1000 0'
    end_point = '500 -1000 0'
    num_points = 51
    sort_by = x
  []
  # Temperature vs. depth along the well axis (r=0) - context for how the geothermal gradient
  # and the well's own thermal interaction combine along the column.
  [temperature_along_well_axis]
    type = LineValueSampler
    variable = temperature
    start_point = '0 0 0'
    end_point = '0 -1000 0'
    num_points = 41
    sort_by = y
  []
  # Pressure vs. depth along the well axis (r=0), full column - the direct, simulation-grounded
  # picture of how the wellbore pressure profile differs between the constant-unit_weight and
  # unit_weight_fp treatments (compare production_out vs. production_constant_unit_weight_out).
  # num_points matches the well's own 100m point spacing (peaceman_wellbore.bh) - oversampling
  # between the discrete Peaceman points picks up local near-sink pressure structure that
  # doesn't cancel cleanly between the two variants, adding noise without adding information.
  [porepressure_along_well_axis]
    type = LineValueSampler
    variable = porepressure
    start_point = '0 0 0'
    end_point = '0 -2000 0'
    num_points = 21
    sort_by = y
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -ksp_gmres_restart -pc_type -sub_pc_type -sub_pc_factor_shift_type -pc_asm_overlap'
    petsc_options_value = 'gmres 30 asm ilu NONZERO 2'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  start_time = 0
  end_time = 1.57788e8 # 5 years
  dtmin = 1
  dtmax = 3.15576e7 # 1 year
  nl_abs_tol = 1e-6
  nl_rel_tol = 1e-8
  l_tol = 1e-4
  l_max_its = 200
  nl_max_its = 20
  automatic_scaling = true
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 3600
    optimal_iterations = 8
    growth_factor = 1.5
    cutback_factor = 0.5
  []
[]

[Outputs]
  csv = true
  print_linear_residuals = false
[]
