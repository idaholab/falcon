T_inj = 323.15

T_top = 327
T_bottom = 550

p_out = 1.5e7

well_length = 1000
well_radius = 0.1
well_A = ${fparse pi * well_radius * well_radius}
well_D_h = ${fparse 2 * well_radius}
well_P_hf = ${fparse 2 * pi * well_radius}

[GlobalParams]
  gravity_vector = '0 0 -9.81'

  initial_vel = 0
  initial_p = ${p_out}
  initial_T = initial_T_fn

  closures = simple

  n_elems = 100
[]

[Closures]
  [simple]
    type = Closures1PhaseSimple
  []
[]

[Modules]
  [FluidProperties]
    [./water]
      type = IAPWS95LiquidFluidProperties
    [../]
  []
[]

[HeatStructureMaterials]
  [./mild_steel]
    type = SolidMaterialProperties
    rho = 7850
    cp = 510.8
    k = 60
  [../]

  [./cement]
    type = SolidMaterialProperties
    rho = 2400
    cp = 920
    k = 0.29
  [../]

  [./rock]
    type = SolidMaterialProperties
    rho = 2750
    cp = 830
    k = 3
  [../]
[]

[Functions]
  [./initial_T_fn]
    type = PiecewiseLinear
    axis = z
    x = '-${well_length} 0'
    y = '${T_bottom} ${T_top}'
  [../]

  [./inlet_m_dot_fn]
    type = PiecewiseLinear
    x = '0 120'
    y = '0 5'
  [../]

  [./T_ext_fn]
    type = PiecewiseLinear
    axis = z
    x = '-${well_length} 0'
    y = '${T_bottom} ${T_top}'
  [../]
[]

[Components]
  [./inlet]
    type = InletMassFlowRateTemperature1Phase
    input = 'pipe:in'
    m_dot = 5
    T = ${T_inj}
  [../]

  [./pipe]
    type = FlowChannel1Phase
    fp = water
    position = '0 0 0'
    orientation = '0 0 -1'
    A = ${well_A}
    D_h = ${well_D_h}

    f = 0.01

    length = ${well_length}
  [../]

  [./surr]
    type = HeatStructureCylindrical
    position = '0 0 0'
    orientation = '0 0 -1'
    length = ${well_length}

    names = 'wall cement rock'
    widths = '0.00635 5.10E-02 4.84265'
    materials = 'mild_steel cement rock'
    n_part_elems = '10 5 5'
    inner_radius = ${well_radius}
    initial_T = initial_T_fn
  [../]

  [./hx_pipe_surr]
    type = HeatTransferFromHeatStructure1Phase
    flow_channel = pipe
    hs = surr
    hs_side = INNER
    P_hf = ${well_P_hf}
    Hw = 1000
  [../]

  [./T_ext]
    type = HSBoundarySpecifiedTemperature
    hs = surr
    boundary = surr:outer
    T = T_ext_fn
  [../]

  [./outlet]
    type = Outlet1Phase
    input = 'pipe:out'
    p = ${p_out}
  [../]
[]

[ControlLogic]
  [./inlet_m_dot]
    type = TimeFunctionComponentControl
    component = inlet
    parameter = m_dot
    function = inlet_m_dot_fn
  [../]
[]

[Preconditioning]
  [./pc]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  start_time = 0
  end_time = 172800
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.1
  [../]

  solve_type = 'NEWTON'
  line_search = basic
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-8
  nl_max_its = 10
  l_tol = 1e-3
  l_max_its = 10

  automatic_scaling = true
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  velocity_as_vector = false
[]
