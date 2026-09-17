# Analytic check for the FunctionEnthalpySink DiracKernel.
#
# FunctionEnthalpySink is identical to EnthalpySink (see enthalpy_sink/inp.i for
# further discussion of the setup, including why the [PorousFlowUnsaturated]
# action is used to build the porous-flow materials instead of a hand-rolled
# [Materials] block), except that the inlet temperature is given by a function
# of time and position instead of a fixed constant:
#   h = fp.h_from_p_T(pressure, function.value(t, point))
# FunctionEnthalpySink::computeQpBaseOutflow evaluates the function at the
# CURRENT time _t (i.e. the end of the current timestep, consistent with
# backward-Euler), so with a function T_in(t) = A + B*t the exact outflow
# during the step ending at time t_n is:
#   RATE * line_length * cv * (A + B*t_n) * dt
# ("fluxes" is per-unit-length, see enthalpy_sink/inp.i for why the
# line_length factor appears.)
#
# Values used: A = 300, B = 0.5, RATE = 0.05, line_length = 0.3, cv = 4000,
# dt = 100. The "residual" postprocessor is
# abs(outflow_energy - RATE*line_length*cv*(A + B*t)*dt), which should be ~0
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
    porepressure_coefficient = 0 # forces h_from_p_T(P,T) = cv*T exactly (see class doc)
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
  [outflow_energy_uo]
    type = PorousFlowSumQuantity
  []
[]

[DiracKernels]
  [source_h]
    type = FunctionEnthalpySink
    variable = temperature
    fp = simple_fluid
    pressure = pressure
    function = t_in_fn
    fluid_phase = 0
    line_base = '1 0.5 0.5'
    line_direction = '0 1 0'
    line_length = 0.3
    p_or_t_vals = '0 1e10'
    fluxes = '0.05 0.05'
    SumQuantityUO = outflow_energy_uo
  []
[]

[Postprocessors]
  [outflow_energy]
    type = PorousFlowPlotQuantity
    uo = outflow_energy_uo
  []
  [residual]
    type = ParsedPostprocessor
    pp_names = 'outflow_energy'
    use_t = true
    expression = 'abs(outflow_energy - 0.05 * 0.3 * 4000 * (300 + 0.5*t) * 100)'
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
