# Analytic check for the PointEnthalpySourceFromFunction DiracKernel.
#
# PointEnthalpySourceFromFunction evaluates the fluid enthalpy at a
# FUNCTION-given temperature (not the local solution temperature):
#   h = fp.h_from_p_T(pressure, function.value(t, point))
# so, unlike PointEnthalpySink, h here is a known function of time alone
# (SimpleFluidProperties with porepressure_coefficient=0 gives
# h_from_p_T(P,T) = cv*T exactly -- see EnthalpySink.md).
#
# Using the same energy-conservation argument as point_enthalpy_sink/inp.i
# (uniform pressure => no advection; insulated boundary => conduction
# integrates to zero over the whole domain), the change in the domain's
# total heat energy (PorousFlowHeatEnergy, rock + fluid) over a timestep
# ending at t_n exactly equals the DiracKernel's own contribution:
#   -mass_flux * cv * (A + B*t_n) * dt
# with the same empirically-confirmed sign as PointEnthalpySink (positive
# mass_flux decreases the domain's heat energy, despite the class name).
#
# "residual" = abs(delta_E + mass_flux*cv*(A+B*t)*dt) should be ~0 (to
# solver/roundoff tolerance) at every timestep except t=0 (see
# point_enthalpy_sink/inp.i for why the "initial" row is an exception).

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
  [t_in_fn]
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

[DiracKernels]
  [source_h]
    type = PointEnthalpySourceFromFunction
    variable = temperature
    mass_flux = mass_flux_pp
    fp = simple_fluid
    pressure = pressure
    function = t_in_fn
    point = '0.5 0.5 0'
  []
[]

[Postprocessors]
  [mass_flux_pp]
    type = ConstantPostprocessor
    value = 0.01
  []
  [heat_energy]
    type = PorousFlowHeatEnergy
    phase = '0'
    execute_on = 'initial timestep_end'
  []
  [delta_E]
    type = ChangeOverTimePostprocessor
    postprocessor = heat_energy
    execute_on = 'initial timestep_end'
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'delta_E'
    use_t = true
    expression = 'abs(delta_E + 0.01 * 4000 * (300 + 0.5*t) * 100)'
    execute_on = 'initial timestep_end'
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
