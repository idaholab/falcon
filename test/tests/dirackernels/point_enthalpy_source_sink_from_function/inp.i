# Analytic check for the PointEnthalpySourceSinkFromFunction DiracKernel
# (injection branch).
#
# PointEnthalpySourceSinkFromFunction picks one of two behaviours each
# residual evaluation, based on the signs of mass_flux_function(t) and
# temperature_function(t):
#  - extraction (mass_flux>=0 and T_input<=0): h evaluated at the LOCAL
#    solution temperature;
#  - injection (mass_flux<=0 and T_input>0): h evaluated at the function's
#    temperature;
#  - any other combination: mooseError "not coordinated" (see the
#    RunException test in the "tests" spec file).
#
# This test only exercises the INJECTION branch, where h is a known function
# of time alone (avoiding the local-temperature dependence of the extraction
# branch -- see point_enthalpy_sink/inp.i for how that dependence is handled
# for a class that always uses the local temperature).
#
# mass_flux_function(t) = -0.02 (kg/s, negative = injection),
# temperature_function(t) = 300 + 0.5*t (K).
# h = cv*T = 4000*(300+0.5*t) exactly (porepressure_coefficient=0, see
# EnthalpySink.md). The DiracKernel accumulates
#   test[i][qp] * mass_flux * h * dt
# into the SumQuantityUO every residual evaluation; since the FE shape
# functions sum to unity, the total accumulated over the timestep ending at
# t_n is exactly mass_flux * h(t_n) * dt, read directly via
# PorousFlowPlotQuantity.
#
# "residual" = abs(outflow - (-0.02) * 4000*(300+0.5*t) * dt) should be ~0
# (to solver/roundoff tolerance) at every timestep.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 1
[]

[GlobalParams]
  PorousFlowDictator = dictator
[]

[Variables]
  [pressure]
    initial_condition = 1e6
  []
  [temperature]
    initial_condition = 300
  []
[]

[Functions]
  [mass_flux_fn]
    type = ParsedFunction
    expression = '-0.02'
  []
  [temperature_fn]
    type = ParsedFunction
    expression = '300 + 0.5*t'
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    bulk_modulus = 2e9
    density0 = 1000
    thermal_expansion = 0
    cv = 4000
    porepressure_coefficient = 0 # forces h_from_p_T(P,T) = cv*T exactly
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
  [outflow_uo]
    type = PorousFlowSumQuantity
  []
[]

[DiracKernels]
  [source_h]
    type = PointEnthalpySourceSinkFromFunction
    variable = temperature
    SumQuantityUO = outflow_uo
    mass_flux_function = mass_flux_fn
    temperature_function = temperature_fn
    pressure = pressure
    fp = simple_fluid
    point = '0.5 0.5 0'
  []
[]

[Postprocessors]
  [outflow]
    type = PorousFlowPlotQuantity
    uo = outflow_uo
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'outflow'
    use_t = true
    expression = 'abs(outflow - (-0.02) * 4000 * (300 + 0.5*t) * 100)'
  []
[]

[Preconditioning]
  [preferred]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type'
    petsc_options_value = ' lu     '
  []
[]

[Executioner]
  type = Transient
  solve_type = Newton
  nl_abs_tol = 1e-10
  dt = 100
  end_time = 300
[]

[Outputs]
  csv = true
  execute_on = 'initial timestep_end'
[]
