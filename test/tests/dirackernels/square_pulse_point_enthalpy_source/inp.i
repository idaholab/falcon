# Analytic check for the PorousFlowSquarePulsePointEnthalpySource DiracKernel.
#
# This class adds heat energy at a constant mass flux and fixed inlet
# temperature T_in, but only during [start_time, end_time], with the mass
# flux ramped smoothly across the timestep(s) that straddle start_time or
# end_time so that the total added mass (and hence energy) over the whole
# pulse is exact regardless of the timestep size. There are six cases for
# how (start_time, end_time) relate to the current step (t-dt, t) -- see the
# comment block in PorousFlowSquarePulsePointEnthalpySource::computeQpResidual.
# Denoting that fraction "factor" (0 <= factor <= 1):
#   1) t < start_time                        : factor = 0            ("before")
#   2) t-dt < start_time <= t <= end_time     : factor = (t-start_time)/dt   ("ramp up")
#   3) t-dt < start_time and t > end_time     : factor = (end_time-start_time)/dt  (pulse
#                                                entirely inside this one step)
#   4) start_time <= t-dt and t <= end_time   : factor = 1            ("fully inside")
#   5) start_time <= t-dt < end_time < t      : factor = (end_time-(t-dt))/dt ("ramp down")
#   6) t-dt >= end_time                       : factor = 0            ("after")
# Cases 1,2,4,5,6 are exercised directly below by choosing a constant dt=100
# with start_time=250, end_time=650 (so the pulse spans several steps).
# Case 3 shares the same "t-dt < start_time" branch as case 2 (differing only
# in the trailing else), so is not separately re-derived here; its formula is
# identical in form to case 2's, just with the pulse's own (end_time -
# start_time) in place of (t - start_time).
#
# Using the same energy-conservation argument as point_enthalpy_sink/inp.i,
# and since T_in is a fixed input (h = cv*T_in is exactly known, see
# EnthalpySink.md), the exact change in the domain's total heat energy over
# the timestep ending at t_n is:
#   +mass_flux * cv * T_in * factor(t_n) * dt
# (the sign, opposite to PointEnthalpySink/PorousFlowSquarePulsePointEnthalpySink,
# was confirmed empirically: this class's computeQpResidual has an explicit
# minus sign that the Sink classes do not have, and positive mass_flux here
# increases the domain's heat energy, consistent with the class being a
# "Source".)
#
# "residual" = abs(delta_E - mass_flux*cv*T_in*factor(t)*dt) should be ~0 (to
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
    type = PorousFlowSquarePulsePointEnthalpySource
    variable = temperature
    mass_flux = 0.01
    fp = simple_fluid
    pressure = pressure
    T_in = 320
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
    pp_names = 'delta_E factor dt_pp'
    expression = 'abs(delta_E - 0.01 * 4000 * 320 * factor * dt_pp)'
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
