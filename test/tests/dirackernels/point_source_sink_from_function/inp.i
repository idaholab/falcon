# Analytic check for the PointSourceSinkFromFunction DiracKernel.
#
# PointSourceSinkFromFunction is a pure mass source/sink: at each residual
# evaluation it computes mass_flux = function.value(t, point) and adds
#   _total_outflow_mass.add(test[i][qp] * mass_flux * dt)
# summed over the quadrature points/test functions of the element containing
# the point. Since the FE shape functions sum to unity at any point
# (partition of unity), this sum is exactly mass_flux(t_n) * dt for the
# timestep ending at t_n, regardless of which node(s) of the mesh the point
# happens to land on. PorousFlowPlotQuantity reads this accumulated value
# directly from the SumQuantityUO.
#
# mass_flux_function(t) = 0.02 + 0.0001*t (kg/s), so the exact accumulated
# value for the timestep ending at t_n, with step size dt, is:
#   (0.02 + 0.0001*t_n) * dt
# "residual" = abs(outflow - (0.02+0.0001*t)*dt) should be ~0 (to
# solver/roundoff tolerance) at every timestep.

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
[]

[Functions]
  [mass_flux_fn]
    type = ParsedFunction
    expression = '0.02 + 0.0001*t'
  []
[]

[Kernels]
  [mass_dot]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = pressure
  []
[]

[UserObjects]
  [dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'pressure'
    number_fluid_phases = 1
    number_fluid_components = 1
  []
  [pc]
    type = PorousFlowCapillaryPressureConst
  []
  [outflow_uo]
    type = PorousFlowSumQuantity
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    bulk_modulus = 2e9
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
    porepressure = pressure
    capillary_pressure = pc
  []
  [massfrac]
    type = PorousFlowMassFraction
  []
  [fluid_props]
    type = PorousFlowSingleComponentFluid
    phase = 0
    fp = simple_fluid
  []
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1
  []
[]

[DiracKernels]
  [source]
    type = PointSourceSinkFromFunction
    variable = pressure
    mass_flux_function = mass_flux_fn
    point = '0.5 0.5 0'
    SumQuantityUO = outflow_uo
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
    expression = 'abs(outflow - (0.02 + 0.0001*t) * 100)'
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
