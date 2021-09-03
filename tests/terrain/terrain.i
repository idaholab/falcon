[Mesh/terrain]
  type = FileMeshGenerator
  file = terrain.msh
[]

[Variables]
  [u]
    initial_condition = 300
  []
[]

[Kernels]
  [time]
    type = ADTimeDerivative
    variable = u
  []
  [diff_upper]
    type = ADMatDiffusion
    variable = u
    block = granitoid_40m_surface_40m
    diffusivity = 50000
  []
  [diff_lower]
    type = ADMatDiffusion
    variable = u
    block = bottom_40m_granitoid_40m
    diffusivity = 1000
  []
[]

[BCs]
  [bottom]
    type = ADDirichletBC
    variable = u
    boundary = 'bottom_40m_granitoid_40m_front granitoid_40m_surface_40m_front'
    value = 300
  []
  [top]
    type = ADNeumannBC
    variable = u
    boundary = 'bottom_40m_granitoid_40m_back granitoid_40m_surface_40m_back'
    value = 100
  []
[]

[Executioner]
  type = Transient
  num_steps = 200
  nl_rel_tol = 1e-10
  solve_type = 'NEWTON'
  steady_state_detection= true
  steady_state_tolerance = 1e-6
  [TimeStepper]
    type = IterationAdaptiveDT
    optimal_iterations = 4
    growth_factor = 1.2
    dt = 1
  []
[]

[Outputs]
  exodus = true
[]
