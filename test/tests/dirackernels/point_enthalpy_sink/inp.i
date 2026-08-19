# Analytic check for the PointEnthalpySink DiracKernel.
#
# Unlike EnthalpySink/FunctionEnthalpySink, PointEnthalpySink evaluates the
# fluid enthalpy at the LOCAL (solution-dependent) temperature:
#   h = fp.h_from_p_T(pressure, PorousFlow_temperature_qp)
# so h is not known a priori. Instead of assuming h is constant, this test
# verifies energy conservation directly: since
#  - pressure is uniform for all time (uniform IC, no BC, no other source
#    touches pressure, gravity = 0), so there is no advective flux of mass or
#    heat,
#  - the domain boundary is insulated (no BCs on temperature => zero-flux
#    natural boundary condition), so heat conduction only redistributes
#    energy internally and integrates to zero over the whole domain,
# the only way the domain's total heat energy (PorousFlowHeatEnergy, rock +
# fluid) can change during a timestep is via the DiracKernel source. Because
# PointEnthalpySink's residual (test*mass_flux*h) is added directly into the
# same discretized (backward-Euler) equation as PorousFlowEnergyTimeDerivative,
# the change in total heat energy over one timestep exactly equals
#   -mass_flux * cv * T_local(t_n) * dt
# where T_local(t_n) is the (converged) local temperature at the Dirac point
# at the END of the timestep (consistent with backward-Euler), cv is the
# fluid's specific heat (SimpleFluidProperties with porepressure_coefficient=0
# gives h = cv*T exactly, see EnthalpySink.md for the derivation), and the
# minus sign was determined empirically (see below) -- positive mass_flux
# acts as a SINK, decreasing the domain's heat energy.
#
# "residual" = abs(delta_E + mass_flux*cv*T_local*dt) should be ~0 (to
# solver/roundoff tolerance) at every timestep. (The t=0 "initial" row is an
# exception: ChangeOverTimePostprocessor has no previous step to compare to
# there, so delta_E is trivially 0 and "residual" is just the t=0 T_local
# term; the physically meaningful rows are t=100,200,300.)

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
    type = PointEnthalpySink
    variable = temperature
    mass_flux = mass_flux_pp
    fp = simple_fluid
    pressure = pressure
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
    phase = '0' # include the fluid's internal energy as well as the rock's
    execute_on = 'initial timestep_end'
  []
  [delta_E]
    type = ChangeOverTimePostprocessor
    postprocessor = heat_energy
    execute_on = 'initial timestep_end'
  []
  [T_local]
    type = PointValue
    variable = temperature
    point = '0.5 0.5 0'
    execute_on = 'initial timestep_end'
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'delta_E T_local'
    expression = 'abs(delta_E + 0.01 * 4000 * T_local * 100)'
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
