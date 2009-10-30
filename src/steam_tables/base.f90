subroutine base ( t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub )

!*****************************************************************************80
!
!! BASE calculates quantities associated with the base Helmholtz function.
!
!  Discussion:
!
!    The equation for the base Helmholtz function AB(T,RHO) is:
!
!      AB(T,RHO) = R * T * (
!        - ln ( 1 - y )
!        - ( beta - 1 ) / ( 1 - y )
!        + ( alpha + beta + 1 ) / ( 2 * ( 1 - y )**2 )
!        + 4 * y * ( ( Bbar / b ) - gamma )
!        - 0.5 * ( alpha - beta + 3 )
!        + ln ( RHO * R * T / P0 ) )
!                                                      (Equation 2)
!   where
!
!     y = b * rho / 4,
!     alpha = 11,
!     beta = 133/3,
!     gamma = 7/2,
!     P0 = 0.101325 MegaPascals = 1 atm
!
!   and
!
!     b(T) = b1 * ln(T/T0) + sum(j=0,1,3,5) b(j)*(T0/T)**j  (Equation 3)
!
!     Bbar(T) = sum(j=0,1,2,4) B(j)*(T0/T)**j               (Equation 4).
!
!   where
!
!     T0=647.073 K and the coefficients b(j) and B(j) are
!
!     j    b(j)                         B(j)
!    --    -----------                  ----------
!     0    0.7478629                    1.1278334
!     1   -0.3540782                   -0.5944001
!     2    0                           -5.010996
!     3    0.007159876                  0
!     4    0                            0.63684256
!     5   -0.003528426                  0
!
!  For the derived quantities, the following relations are used:
!
!    Pressure:                  PB      = RHO**2 * dAB/dRHO
!    Density derivative:        DPDRB   = 2*PB/RHO + RHO**2 * d2AB/dRHO2
!    Temperature derivative:    DPDTB   = RHO**2 * d2AB/(dRHO dT)
!    Specific entropy:          SB      = ( UB - AB ) / T
!    Specific internal energy:  UB      = AB + T * SB
!    Specific enthalpy:         HB      = UB + PB / RHO
!    Specific heat capacity
!      at constant volume:      CVB     = - T * d2AB/dT2
!    Specific Gibbs function:   GB      = AB + PB / RHO
!
!  Modified:
!
!    03 February 2002
!
!  Reference:
!
!    Lester Haar, John Gallagher and George Kell,
!    NBS/NRC Steam Tables:
!    Thermodynamic and Transport Properties and Computer Programs
!      for Vapor and Liquid States of Water in SI Units,
!    Hemisphere Publishing Corporation, Washington, 1984,
!    TJ270.H3
!
!    C A Meyer, R B McClintock, G J Silvestri, R C Spencer,
!    ASME Steam Tables: Thermodynamic and Transport Properties of Steam,
!    American Society of Mechanical Engineers, 1967.
!
!  Parameters:
!
!    Input, real ( kind = 8 ) T, the temperature, in degrees Kelvin.
!
!    Input, real ( kind = 8 ) RHO, the density, in G/CM3.
!
!    Output, real ( kind = 8 ) AB, the base value of the Helmholtz function,
!    in KJ/kg.
!
!    Output, real ( kind = 8 ) CVB, the base value of the isochoric (constant
!    volume) heat capacity, in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) DPDRB, the base value of the partial
!    derivative dP(T,RHO)/dRHO, with T held fixed, in (MegaPascals CM3)/G.
!
!    Output, real ( kind = 8 ) DPDTB, the base value of the partial
!    derivative dP(T,RHO)/dT, with RHO held fixed, in
!    MegaPascals/degrees Kelvin.
!
!    Output, real ( kind = 8 ) GB, the base value of the Gibbs free energy,
!    in KJ/kg.
!
!    Output, real ( kind = 8 ) HB, the base value of enthalpy, in KJ/kg.
!
!    Output, real ( kind = 8 ) PB, the base pressure, in MegaPascals.
!
!    Output, real ( kind = 8 ) SB, the base value of entropy,
!    in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) UB, the base value of internal energy,
!    in KJ/kg.
!
  implicit none

  real ( kind = 8 ) ab
  real ( kind = 8 ), parameter :: alpha = 11.0D+00
  real ( kind = 8 ) b1
  real ( kind = 8 ) b1t
  real ( kind = 8 ) b1tt
  real ( kind = 8 ) b2
  real ( kind = 8 ) b2t
  real ( kind = 8 ) b2tt
  real ( kind = 8 ), parameter :: beta = 44.333333333333D+00
  real ( kind = 8 ) cvb
  real ( kind = 8 ) dpdrb
  real ( kind = 8 ) dpdtb
  real ( kind = 8 ) dz
  real ( kind = 8 ) dz0
  real ( kind = 8 ), parameter :: gamma = 3.5D+00
  real ( kind = 8 ) gascon
  real ( kind = 8 ) gb
  real ( kind = 8 ) hb
  real ( kind = 8 ), parameter :: p_zero = 0.101325D+00
  real ( kind = 8 ) pb
  real ( kind = 8 ) rho
  real ( kind = 8 ) sb
  real ( kind = 8 ) t
  real ( kind = 8 ) ub
  real ( kind = 8 ) x
  real ( kind = 8 ) y
  real ( kind = 8 ) z
  real ( kind = 8 ) z0
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'BASE - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'BASE - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was RHO = ', rho
    stop
  end if
!
!  Compute auxilliary quantities for Equation 2.
!
  call bb ( t, b1, b2, b1t, b2t, b1tt, b2tt )

  y = 0.25D+00 * b1 * rho

  x = 1.0D+00 - y
!
!  Evaluate Equation 2.
!
  ab =   - log ( 1.0D+00 - y ) &
         - ( beta - 1.0D+00 ) / ( 1.0D+00 - y ) &
         + ( alpha + beta + 1.0D+00 ) / ( 2.0D+00 * ( 1.0D+00 - y )**2 ) &
         + 4.0D+00 * y * ( ( b2 / b1 ) - gamma ) &
         - 0.5D+00 * ( alpha - beta + 3.0D+00 ) &
         + log ( rho * gascon() * t / p_zero )
!
!  Determine quantities defined in terms of AB.
!
  pb = ( 1.0D+00 + alpha * y + beta * y**2 ) / ( 1.0D+00 - y )**3 &
    + 4.0D+00 * y * ( b2 / b1 - gamma )

  z0 = ( 1.0D+00 + alpha * y + beta * y**2 ) / ( 1.0D+00 - y )**3

  z = z0 + 4.0D+00 * y * ( b2 / b1 - gamma )

  dz0 = ( alpha + 2.0D+00 * beta * y ) / ( 1.0D+00 - y )**3 &
    + 3.0D+00 * ( 1.0D+00 + alpha * y + beta * y**2 ) / ( 1.0D+00 - y )**4

  dz = dz0 + 4.0D+00 * ( b2 / b1 - gamma )

  gb = ab + pb

  ub = - t * b1t * ( pb - 1.0D+00 - rho * b2 ) / b1 - rho * t * b2t

  hb = pb + ub
!
!  An incorrect version of this equation began:
!
!    cvb = 2.0D+00 * ub + ( pb - 1.0D+00 ) &
!
!  and caused me no end of trouble.  My fault, JVB, 03 February 2002
!
  cvb = 2.0D+00 * ub + ( z0 - 1.0D+00 ) &
    * ( ( t * b1t / b1 )**2 - t**2 * b1tt / b1 ) &
    - rho * t**2 * ( b2tt - gamma * b1tt ) - ( t * b1t / b1 )**2 * y * dz0

  dpdtb = pb / t + rho * ( 0.25D+00 * ( dz0 + 4.0D+00 * ( b2 / b1 - gamma ) ) &
    * b1t + b2t - b2 / b1 * b1t )

  sb = ub - ab

  dpdrb = pb + y * ( dz0 + 4.0D+00 * ( b2 / b1 - gamma ) )
!
!  Assign dimensions.
!
  ab =    gascon() * t       * ab
  cvb =   gascon()           * cvb
  dpdrb = gascon() * t       * dpdrb
  dpdtb = gascon() * t * rho * dpdtb
  gb =    gascon() * t       * gb
  hb =    gascon() * t       * hb
  pb =    gascon() * t * rho * pb
  sb =    gascon()           * sb
  ub =    gascon() * t       * ub

  return
end
