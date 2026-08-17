# Analytic check for the EnthalpySink DiracKernel.
#
# EnthalpySink layers an enthalpy multiplier on top of PorousFlowPolyLineSink:
# it removes mass at a rate given by a piecewise-linear function of the local
# porepressure (the usual PorousFlowPolyLineSink behaviour), and multiplies
# that mass outflow by the fluid enthalpy evaluated at (local pressure, T_in).
#
# NOTE on setup: a hand-rolled minimal [Materials] block (mimicking other
# dirackernels tests in this directory) was tried first, but PorousFlowLineSink
# (the base class of EnthalpySink) reports "you do not have a quadpoint
# porepressure material" when EnthalpySink's own required "fp"/"pressure"
# parameters are present, even though the identical [Materials] block works
# fine for a bare PorousFlowPolyLineSink. Building the porous-flow materials
# via the [PorousFlowUnsaturated] action (the pattern used by the working
# examples/rtes_* input files that already use EnthalpySink) avoids the
# problem, so that action is used here instead.
#
# To get an exact hand-derivable result:
#  - "fluxes" is a FLAT table ('RATE RATE'), so the mass outflow rate is
#    exactly RATE = 0.05 kg/s regardless of the local porepressure.
#  - SimpleFluidProperties is used with porepressure_coefficient = 0, which
#    forces h_from_p_T(P,T) = e_from_p_T(P,T) = cv*T exactly (see
#    SimpleFluidProperties::h_from_p_T), independent of pressure/density.
#  - T_in is a fixed constant, so h = cv*T_in = 4000*320 = 1.28e6 J/kg exactly.
#
# Note "fluxes" is a flux PER UNIT LENGTH (kg.m^-1.s^-1), and PorousFlowLineSink
# multiplies it by "line_length" (0.3 m here) to get the actual mass rate in
# kg/s, i.e. mass rate = RATE * line_length = 0.05 * 0.3 = 0.015 kg/s.
#
# Hence the exact energy removed from the domain over one timestep of length
# dt is: RATE * line_length * cv * T_in * dt
#      = 0.05 * 0.3 * 4000 * 320 * 100 = 1.92e6 J.
# PorousFlowLineSink (the base class) accumulates exactly this quantity, each
# timestep, into the SumQuantityUO, which PorousFlowPlotQuantity reports as
# "outflow_energy". The "residual" postprocessor below is
# abs(outflow_energy - analytic) and should be ~0 (to solver/roundoff
# tolerance) at every timestep.

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
    type = EnthalpySink
    variable = temperature
    fp = simple_fluid
    pressure = pressure
    T_in = 320
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
    expression = 'abs(outflow_energy - 0.05 * 0.3 * 4000 * 320 * 100)'
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
