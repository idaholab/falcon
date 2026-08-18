# Analytic check for the PorousFlowSquarePulsePointEnthalpySink DiracKernel.
#
# This class is the sink counterpart of PorousFlowSquarePulsePointEnthalpySource
# (see square_pulse_point_enthalpy_source/inp.i for the derivation of the
# 6-case pulse ramp "factor", 5 of which are exercised here with a constant
# dt=100 and start_time=250, end_time=650), except that:
#  - it removes (rather than adds) heat energy: its computeQpResidual has no
#    leading minus sign (matching PointEnthalpySink), which was empirically
#    confirmed to decrease the domain's heat energy for positive mass_flux
#    (see point_enthalpy_sink/inp.i);
#  - h is evaluated at the LOCAL solution temperature, not a fixed T_in, so
#    the same energy-conservation approach as point_enthalpy_sink/inp.i is
#    used (comparing against T_local(t) rather than a constant).
#
# "residual" = abs(delta_E + mass_flux*cv*T_local(t)*factor(t)*dt) should be
# ~0 (to solver/roundoff tolerance) at every timestep except t=0 (see
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
    type = PorousFlowSquarePulsePointEnthalpySink
    variable = temperature
    mass_flux = 0.01
    fp = simple_fluid
    pressure = pressure
    point = '0.5 0.5 0'
    start_time = 250
    end_time = 650
  []
[]

[Postprocessors]
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
  [dt_pp]
    type = TimestepSize
    execute_on = 'initial timestep_end'
  []
  [T_local]
    type = PointValue
    variable = temperature
    point = '0.5 0.5 0'
    execute_on = 'initial timestep_end'
  []
  [factor]
    type = ParsedPostprocessor
    pp_names = 'dt_pp'
    use_t = true
    expression = 'if(t < 250, 0,
                  if(t - dt_pp >= 650, 0,
                  if(t - dt_pp < 250,
                    if(t <= 650, (t-250)/dt_pp, (650-250)/dt_pp),
                    if(t <= 650, 1, (650-(t-dt_pp))/dt_pp))))'
    execute_on = 'initial timestep_end'
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'delta_E factor dt_pp T_local'
    expression = 'abs(delta_E + 0.01 * 4000 * T_local * factor * dt_pp)'
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
  [TimeStepper]
    type = ConstantDT
    dt = 100
  []
  end_time = 800
[]

[Outputs]
  csv = true
  execute_on = 'initial timestep_end'
[]
