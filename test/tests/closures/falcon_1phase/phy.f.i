# This test makes sure that the right closure for wall friction is used through
# the closure class when running 1-phase flow problem.

[GlobalParams]
  gravity_vector = '-9.81 0 0'

  initial_T = 372.7
  initial_p = 1e5
  initial_vel = 0
[]

[Closures]
  [Falcon]
    type = Closures1PhaseFalcon
  []
[]

[FluidProperties]
  [./fp]
    type = IAPWS95LiquidFluidProperties
  [../]
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

    fp = fp
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

[Preconditioning]
  [./SMP_PJFNK]
    type = SMP
    full = true
  [../]
[]

[VectorPostprocessors]
  [./f]
    type = ADSampler1DReal
    block = pipe
    property = 'f_D'
    sort_by = x
  [../]
[]


[Executioner]
  type = Transient
  scheme = 'bdf2'

  start_time = 0
  dt = 0.1
  num_steps = 1
  abort_on_solve_fail = true

  solve_type = 'PJFNK'
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-8
  nl_max_its = 30

  l_tol = 1e-2
  l_max_its = 30

  automatic_scaling = true
[]

[Outputs]
  [./out]
    type = CSV
    execute_on = final
  [../]
  velocity_as_vector = false
[]
