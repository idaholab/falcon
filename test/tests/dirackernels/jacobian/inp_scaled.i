# Companion to inp.i: the same eight RTES enthalpy DiracKernels, but on a deliberately
# non-dimensionalised problem so that EVERY block of the Jacobian is O(1).
#
# Why a second input exists. PetscJacobianTester gates on ratio_tol, i.e.
# ||J - Jfd||_F / ||J||_F. In inp.i the full PorousFlowUnsaturated action is present, and
# PorousFlowMatrixInternalEnergy (density = 2750, specific_heat_capacity = 830) contributes an
# energy time-derivative diagonal of order 1e5 per node. That single term sets ||J||_F, so the
# absolute error budget at ratio_tol = 1e-7 is ~1e-2 -- while the pressure-block entries these
# kernels actually contribute are mass_flux * dh/dp ~ 1e-5. Three to four orders of magnitude
# separate the two, so a wrong (or missing) pressure off-diagonal is invisible there:
#
#   * the dh/dT diagonals of PointEnthalpySink, PorousFlowSquarePulsePointEnthalpySink and
#     PointEnthalpySourceSinkFromFunction perturb the norm by ~8      -> caught by inp.i
#   * the leading minus of PorousFlowSquarePulsePointEnthalpySource   -> ~6e-6, invisible there
#   * the dh/dp chain rule of EnthalpySink / FunctionEnthalpySink     -> ~6e-6, invisible there
#
# This input follows the pattern of MOOSE's own
# modules/porous_flow/test/tests/jacobian/line_sink02.i: it keeps the PorousFlowDictator and the
# handful of materials the DiracKernels genuinely require, but replaces the action's bulk mass and
# energy kernels with a plain TimeDerivative on each variable, and scales the fluid properties,
# mass fluxes and initial conditions to O(1). With no term dominating, ||J||_F is O(0.1) and the
# pressure-block entries are of the same order, so each defect above moves the ratio by O(1).
#
# porepressure_coefficient is left at its physical default of 1: SimpleFluidProperties computes
# h = e + porepressure_coefficient * p / rho, so setting it to 0 (as the analytic residual tests
# elsewhere in test/tests/dirackernels/ do) would zero dh/dp everywhere and silently remove the
# very term under test.

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
[]

[GlobalParams]
  PorousFlowDictator = dictator
[]

[Variables]
  [pressure]
  []
  [temperature]
  []
[]

[ICs]
  # O(1) ranges, as in line_sink02.i -- these are non-dimensional values chosen to condition the
  # Jacobian, not physical pressures and temperatures.
  [pressure_ic]
    type = RandomIC
    variable = pressure
    min = 0.2
    max = 1.0
  []
  [temperature_ic]
    type = RandomIC
    variable = temperature
    min = 1.0
    max = 2.0
  []
[]

[Kernels]
  # Plain time derivatives in place of the PorousFlow action's mass/energy kernels: they keep the
  # diagonal O(1) instead of O(1e5), which is the whole point of this input.
  [dummy_pressure]
    type = TimeDerivative
    variable = pressure
  []
  [dummy_temperature]
    type = TimeDerivative
    variable = temperature
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    bulk_modulus = 1.5
    density0 = 1
    thermal_expansion = 0
    viscosity = 1
    cv = 1.1
    # porepressure_coefficient left at its default of 1 -- see header comment.
  []
[]

[UserObjects]
  [dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'pressure temperature'
    number_fluid_phases = 1
    number_fluid_components = 1
  []
  [enthalpy_sink_uo]
    type = PorousFlowSumQuantity
  []
  [function_enthalpy_sink_uo]
    type = PorousFlowSumQuantity
  []
  [source_sink_fn_uo]
    type = PorousFlowSumQuantity
  []
  [mass_source_sink_fn_uo]
    type = PorousFlowSumQuantity
  []
[]

[Materials]
  [temperature]
    type = PorousFlowTemperature
    temperature = temperature
  []
  [ppss]
    type = PorousFlow1PhaseFullySaturated
    porepressure = pressure
  []
  [massfrac]
    type = PorousFlowMassFraction
  []
  [simple_fluid]
    type = PorousFlowSingleComponentFluid
    fp = simple_fluid
    phase = 0
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1 0 0  0 1 0  0 0 1'
  []
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1
  []
[]

[Functions]
  [t_in_fn]
    type = ParsedFunction
    expression = '1.4 + 0.2*y'
  []
  [mass_flux_fn]
    type = ParsedFunction
    expression = '0.1'
  []
  [temperature_fn]
    type = ParsedFunction
    expression = '0'
  []
[]

[Postprocessors]
  [mass_flux_pp]
    type = ConstantPostprocessor
    value = 0.1
  []
[]

[DiracKernels]
  [enthalpy_sink]
    type = EnthalpySink
    variable = temperature
    fp = simple_fluid
    pressure = pressure
    T_in = 1.5
    fluid_phase = 0
    line_base = '1 0.1 0.1 0.1'
    line_direction = '0 1 0'
    line_length = 0.2
    # Deliberately NON-flat: PorousFlowPolyLineSink builds its outflow derivative from
    # _sink_func.sampleDerivative(), which is identically 0 for a flat table -- and that would
    # multiply the h * outflowp half of the chain-rule fix by literal zero.
    p_or_t_vals = '0 2'
    fluxes = '0.04 0.06'
    SumQuantityUO = enthalpy_sink_uo
  []
  [function_enthalpy_sink]
    type = FunctionEnthalpySink
    variable = temperature
    fp = simple_fluid
    pressure = pressure
    function = t_in_fn
    fluid_phase = 0
    line_base = '1 0.6 0.1 0.1'
    line_direction = '0 1 0'
    line_length = 0.2
    p_or_t_vals = '0 2'
    fluxes = '0.04 0.06'
    SumQuantityUO = function_enthalpy_sink_uo
  []
  [point_enthalpy_sink]
    type = PointEnthalpySink
    variable = temperature
    mass_flux = mass_flux_pp
    fp = simple_fluid
    pressure = pressure
    point = '0.2 0.8 0.2'
  []
  [square_pulse_sink]
    type = PorousFlowSquarePulsePointEnthalpySink
    variable = temperature
    mass_flux = 0.1
    fp = simple_fluid
    pressure = pressure
    point = '0.5 0.8 0.2'
  []
  [square_pulse_source]
    type = PorousFlowSquarePulsePointEnthalpySource
    variable = temperature
    mass_flux = 0.1
    fp = simple_fluid
    pressure = pressure
    T_in = 1.7
    point = '0.8 0.8 0.2'
  []
  [source_sink_from_function]
    type = PointEnthalpySourceSinkFromFunction
    variable = temperature
    SumQuantityUO = source_sink_fn_uo
    mass_flux_function = mass_flux_fn
    temperature_function = temperature_fn
    pressure = pressure
    fp = simple_fluid
    point = '0.2 0.2 0.8'
  []
  [enthalpy_source_from_function]
    type = PointEnthalpySourceFromFunction
    variable = temperature
    mass_flux = mass_flux_pp
    fp = simple_fluid
    pressure = pressure
    function = t_in_fn
    point = '0.5 0.2 0.8'
  []
  [mass_source_sink_from_function]
    type = PointSourceSinkFromFunction
    variable = pressure
    mass_flux_function = mass_flux_fn
    SumQuantityUO = mass_source_sink_fn_uo
    point = '0.8 0.2 0.8'
  []
[]

[Preconditioning]
  [check]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  solve_type = Newton
  dt = 1
  end_time = 1
[]

[Outputs]
  exodus = false
[]
