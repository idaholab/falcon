[GlobalParams]
  gravity_vector = '0 0 -9.81'

  initial_vel = 0
  initial_p = 1.5e7
  initial_T = initial_T_fn

  closures = simple
[]

[Closures]
  [simple]
    type = Closures1PhaseSimple
  []
[]

[FluidProperties]
  [./water]
    type = IAPWS95LiquidFluidProperties
  [../]
[]

[Functions]
  [./initial_T_fn]
    type = PiecewiseLinear
    axis = z
    x = '0 1000'
    y = '327 550'
  [../]

  [./inlet_m_dot_fn]
    type = PiecewiseLinear
    x = '0 120'
    y = '0 5'
  [../]
[]

[Components]
  [./inlet]
    type = InletMassFlowRateTemperature1Phase
    input = 'pipe:in'
    m_dot = 5
    T = 327
  [../]

  [./pipe]
    type = FlowChannel1Phase
    fp = water
    position = '0 0 0'
    orientation = '0 0 1'
    # r = 0.2 m
    A = 1.2566370614e-01
    D_h = 4.0000000000e-01

    f = 0.01

    length = 1000
    n_elems = 100
  [../]

  [./outlet]
    type = Outlet1Phase
    input = 'pipe:out'
    p = 1.5e7
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
  num_steps = 1
  dt = 0.1

  solve_type = 'NEWTON'
  line_search = basic
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-8
  nl_max_its = 10
  l_tol = 1e-3
  l_max_its = 10
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  velocity_as_vector = false
[]
