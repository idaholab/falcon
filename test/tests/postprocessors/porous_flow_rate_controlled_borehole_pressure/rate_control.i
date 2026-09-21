# Minimal "tank" model (a single element, no Darcy/advective kernel at all - the only physics is
# mass accumulation from the well's own Dirac source) used to check that
# PorousFlowRateControlledBoreholePressure actually converges the delivered rate onto a target,
# using fixed timesteps (no IterationAdaptiveDT) so the secant correction's step-by-step behavior
# is directly comparable against the gold CSV.
#
# Since injected mass has nowhere to go, the reservoir pressure keeps rising every timestep, and
# the correct bottom_p_or_t needed to hold a CONSTANT target rate must keep rising in lockstep -
# this exercises the controller's ability to keep tracking a smoothly-drifting operating point,
# not just settle once and sit still.
[Mesh]
  # Same small single-element "tank" geometry as moose/modules/porous_flow's own bh03.i (a
  # working, well-index-scaled reference point) - see the Materials block below for how reservoir
  # compliance is instead tuned via porosity/bulk_modulus, not mesh size (enlarging the mesh
  # itself, tried during test development, badly distorted the well-index geometry relative to
  # the well radius and made the solve diverge).
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
  xmin = -1
  xmax = 1
  ymin = -1
  ymax = 1
  zmin = -1
  zmax = 1
[]

[GlobalParams]
  PorousFlowDictator = dictator
[]

[Variables]
  [pp]
    initial_condition = 1e6
  []
[]

[Kernels]
  [mass0]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = pp
  []
[]

[UserObjects]
  [dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'pp'
    number_fluid_phases = 1
    number_fluid_components = 1
  []
  [borehole_total_outflow_mass]
    type = PorousFlowSumQuantity
  []
  [borehole_point_flux]
    type = PorousFlowPointFluxQuantity
  []
  [pc]
    type = PorousFlowCapillaryPressureVG
    m = 0.5
    alpha = 1e-7
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    # bulk_modulus lowered by 1e5 from a typical value (2e9), and porosity raised to 0.5 below, so
    # that the modest mass this test injects barely moves reservoir pressure over the run - the
    # Q(P) = A - B*P relationship the controller relies on then stays genuinely quasi-static from
    # one timestep's correction to the next, the way a real (much larger) reservoir would behave.
    bulk_modulus = 2e7
    viscosity = 1e-3
    density0 = 1000
    thermal_expansion = 0
  []
[]

[Materials]
  [temperature]
    type = PorousFlowTemperature
  []
  [ppss]
    type = PorousFlow1PhaseP
    porepressure = pp
    capillary_pressure = pc
  []
  [massfrac]
    type = PorousFlowMassFraction
  []
  [simple_fluid]
    type = PorousFlowSingleComponentFluid
    fp = simple_fluid
    phase = 0
  []
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.5
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1E-12 0 0 0 1E-12 0 0 0 1E-12'
  []
  [relperm]
    type = PorousFlowRelativePermeabilityCorey
    n = 2
    phase = 0
  []
[]

[DiracKernels]
  [bh]
    type = PorousFlowPeacemanBorehole
    variable = pp
    SumQuantityUO = borehole_total_outflow_mass
    PointFluxUO = borehole_point_flux
    point_file = rate_control.bh
    function_of = pressure
    fluid_phase = 0
    bottom_p_or_t = bottom_pressure_control
    unit_weight = '0 0 0'
    use_mobility = true
    character = -1 # injector
  []
[]

[Functions]
  [bottom_pressure_control]
    type = FalconPostprocessorFunction
    pp = bhp_control
  []
  [target_rate]
    type = ParsedFunction
    expression = '-1e-3' # kg/s, negative = injection
  []
[]

[VectorPostprocessors]
  [point_flux]
    type = PorousFlowPlotPointFluxQuantity
    uo = borehole_point_flux
  []
[]

[Postprocessors]
  [injected_mass_rate]
    # A true rate (kg/s) - PorousFlowPointFluxQuantity is NOT multiplied by dt, unlike
    # borehole_total_outflow_mass/PorousFlowSumQuantity.
    type = VectorPostprocessorReductionValue
    vectorpostprocessor = point_flux
    vector_name = flux
    value_type = sum
  []
  [bhp_control]
    type = PorousFlowRateControlledBoreholePressure
    rate_postprocessor = injected_mass_rate
    target_rate = target_rate
    initial_pressure = 1e6
    initial_pressure_step = 1e3
    max_pressure_change = 5e5
    relaxation_factor = 1.0
  []
  [rate_error]
    type = ParsedPostprocessor
    expression = '(injected_mass_rate - (-1e-3)) / 1e-3'
    pp_names = 'injected_mass_rate'
  []
  [p0]
    type = PointValue
    variable = pp
    point = '0 0 0'
    execute_on = timestep_end
  []
[]

[Preconditioning]
  [usual]
    type = SMP
    full = true
    petsc_options = '-snes_converged_reason'
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -ksp_max_it'
    petsc_options_value = 'bcgs bjacobi 1E-6 1E-10 10000 30'
  []
[]

[Executioner]
  type = Transient
  end_time = 1.5
  dt = 0.1
  solve_type = NEWTON
[]

[Outputs]
  file_base = rate_control
  exodus = false
  csv = true
  execute_on = timestep_end
[]
