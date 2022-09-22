[ClosureTest1Phase]
  A = ${A}
  D_h = ${D_h}
  T_wall = ${T_wall}
  T = ${T}
  p = ${p}
  vel = ${vel}
  fp_1phase = fp_1phase

  ad_output = 'Hw'
  ad = true
[]

[FluidProperties]
  [fp_1phase]
    type = IAPWS95LiquidFluidProperties
  []
[]

[Materials]
  [Re_material]
    type = ADReynoldsNumberMaterial
    D_h = D_h
    Re = Re
    mu = mu
    rho = rho
    vel = vel
  []

  [Pr_material]
    type = ADPrandtlNumberMaterial
    cp = cp
    mu = mu
    k = k
  []

  [T_wall_material]
    type = ADCoupledVariableValueMaterial
    prop_name = T_wall
    coupled_variable = T_wall
  []

  [Hw_mat]
    type = ADWallHeatTransferCoefficient3EqnMaterial
    D_h = D_h
    rho = rho
    vel = vel
    v = v
    e = e
    T = T
    T_wall = T_wall
    p = p
    Re = Re
    Pr = Pr
    mu = mu
    fp = fp_1phase
    gravity_magnitude = 9.80665
  []
[]
