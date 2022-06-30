[ClosureTest1Phase]
  A = ${A}
  D_h = ${D_h}
  T_wall = ${T_wall}

  T = ${T}
  p = ${p}
  vel = ${vel}
  fp_1phase = fp_1phase

  output = 'f_F'
[]

[Modules]
  [FluidProperties]
    [./fp_1phase]
      type = IAPWS95LiquidFluidProperties
    [../]
  []
[]

[Materials]
  [./wall_friction_mat]
    type = WallFrictionChurchillMaterial
    rhoA = rhoA
    rhouA = rhouA
    rhoEA = rhoEA
    rho = rho
    vel = vel
    D_h = D_h
    mu = mu
    f_D = f_D
    roughness = 2.5e-6
  [../]

  [./f_F_mat]
    type = FanningFrictionFactorMaterial
    f_F = f_F
    f_D = f_D
  [../]
[]
