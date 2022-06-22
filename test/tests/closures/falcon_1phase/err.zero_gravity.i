[GlobalParams]
  gravity_vector = '0 0 0'

  initial_T = 300
  initial_p = 1e5
  initial_vel = 0
[]

[Closures]
  [Falcon]
    type = Closures1PhaseFalcon
  []
[]

[Modules]
  [FluidProperties]
    [./eos]
      type = IAPWS95LiquidFluidProperties
    [../]
  []
[]

[Components]
  [./pipe]
    type = FlowChannel1Phase

    position = '0 0 0'
    orientation = '1 0 0'
    length = 1
    n_elems = 1
    A    = 1.0e-02
    D_h  = 1.0e-01
    fp = eos
    closures = Falcon
  [../]

  [./inlet]
    type = SolidWall1Phase
    input = 'pipe:in'
  [../]

  [./outlet]
    type = SolidWall1Phase
    input = 'pipe:out'
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  start_time = 0
  dt = 1
  num_steps = 1
  abort_on_solve_fail = true
[]
