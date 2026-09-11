# Jacobian test for the RTES enthalpy DiracKernels reviewed for missing/incorrect Jacobian terms.
#
# How this is run: the PetscJacobianTester in ./tests injects -snes_test_jacobian itself; it is NOT
# supplied by the [Preconditioning] block below, and -snes_type=test has not existed since PETSc
# 3.9. Because run_sim defaults to False the tester also adds
#   -snes_type ksponly -ksp_type preonly -pc_type none -snes_convergence_test skip
# so there is NO nonlinear solve: with Executioner/num_steps=1, exactly one Jacobian is checked, at
# the initial state. One consequence is that only factor = 1.0 of the SquarePulse kernels' six-case
# pulse ramp is ever exercised -- which is why that ramp now lives in a single pulseFactor() helper
# per class rather than being copied into each of computeQpResidual/Jacobian/OffDiagJacobian.
#
# The defects under test:
#   - EnthalpySink, FunctionEnthalpySink: missing computeQpBaseOutflowJacobian override (were
#     missing both the fluid-enthalpy factor and its dh/dp chain-rule term).
#   - PorousFlowSquarePulsePointEnthalpySource: off-diagonal Jacobian was missing a leading minus
#     relative to its residual.
#   - PointEnthalpySink, PorousFlowSquarePulsePointEnthalpySink, PointEnthalpySourceSinkFromFunction
#     (extraction branch): computeQpJacobian() returned 0, discarding a genuine dh/dT diagonal
#     term (these are applied to the temperature variable, and h depends on the local
#     PorousFlow_temperature_qp).
#
# All of the analytic residual tests elsewhere in test/tests/dirackernels/ deliberately set
# SimpleFluidProperties' porepressure_coefficient=0 to get closed-form residuals -- but that also
# zeroes dh/dp everywhere, and (combined with most of those tests using solve=false) means none of
# them exercise a Jacobian at all. This test leaves porepressure_coefficient at its physical
# default of 1.0 so dh/dp is live.
#
# This input keeps the full PorousFlowUnsaturated physics, which makes it representative but also
# means PorousFlowMatrixInternalEnergy's ~1e5 energy diagonal dominates ||J||_F, leaving too coarse
# an absolute budget to see the O(1e-5) pressure-block entries. The three dh/dT diagonals are
# caught here; the pressure-block terms are covered by the companion inp_scaled.i, which is the
# same eight kernels on an O(1)-scaled problem. See that file's header.

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
  [pressure_ic]
    type = RandomIC
    variable = pressure
    min = 1e5
    max = 2e6
  []
  [temperature_ic]
    type = RandomIC
    variable = temperature
    min = 280
    max = 350
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    bulk_modulus = 2e9
    density0 = 1000
    thermal_expansion = 0
    cv = 4000
    # porepressure_coefficient is left at its physical default (1.0) -- see header comment.
  []
[]

[PorousFlowUnsaturated]
  porepressure = pressure
  temperature = temperature
  coupling_type = ThermoHydro
  gravity = '0 0 0'
  fp = simple_fluid
  add_darcy_aux = false
[]

[Materials]
  [thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.5 0 0  0 2.5 0  0 0 2.5'
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1e-15 0 0  0 1e-15 0  0 0 1e-15'
  []
  [rock_heat]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 830.0
    density = 2750
  []
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1
  []
[]

[UserObjects]
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

[Functions]
  [t_in_fn]
    type = ParsedFunction
    expression = '310 + 5*y'
  []
  [mass_flux_fn]
    type = ParsedFunction
    expression = '0.01'
  []
  [temperature_fn]
    type = ParsedFunction
    expression = '0'
  []
[]

[DiracKernels]
  [enthalpy_sink]
    type = EnthalpySink
    variable = temperature
    fp = simple_fluid
    pressure = pressure
    T_in = 320
    fluid_phase = 0
    line_base = '1 0.1 0.1 0.1'
    line_direction = '0 1 0'
    line_length = 0.2
    # Deliberately non-flat: a flat table makes _sink_func.sampleDerivative() identically 0, which
    # multiplies the h * outflowp half of the chain-rule fix by literal zero.
    p_or_t_vals = '0 1e10'
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
    p_or_t_vals = '0 1e10'
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
    mass_flux = 0.01
    fp = simple_fluid
    pressure = pressure
    point = '0.5 0.8 0.2'
  []
  [square_pulse_source]
    type = PorousFlowSquarePulsePointEnthalpySource
    variable = temperature
    mass_flux = 0.01
    fp = simple_fluid
    pressure = pressure
    T_in = 330
    point = '0.8 0.8 0.2'
  []
  [enthalpy_source_from_function]
    # Has a hand-written nonzero pressure off-diagonal and a hard-coded return 0. diagonal; neither
    # was verified by any test before this one.
    type = PointEnthalpySourceFromFunction
    variable = temperature
    mass_flux = mass_flux_pp
    fp = simple_fluid
    pressure = pressure
    function = t_in_fn
    point = '0.5 0.2 0.8'
  []
  [mass_source_sink_from_function]
    # Contributes no Jacobian at all (the residual is a pure Function of time and position); this
    # pins that down rather than leaving it asserted only in a comment.
    type = PointSourceSinkFromFunction
    variable = pressure
    mass_flux_function = mass_flux_fn
    SumQuantityUO = mass_source_sink_fn_uo
    point = '0.8 0.2 0.8'
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
[]

[Postprocessors]
  [mass_flux_pp]
    type = ConstantPostprocessor
    value = 0.01
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
  end_time = 2
[]

[Outputs]
  exodus = false
[]
