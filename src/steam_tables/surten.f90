subroutine surten ( t, sigma )

!*****************************************************************************80
!
!! SURTEN returns the surface tension as a function of temperature.
!
!  Discussion:
!
!    SURTEN uses an equation that yields values of the surface tension to
!    within the accuracy of measurements from the triple point to the
!    critical point.
!
!      Sigma = B * ( (TSTAR-T)/TSTAR)**Mu * (1+b*(TSTAR-T)/TSTAR)
!
!    where:
!
!      TSTAR = 647.15 Degrees Kelvin,
!      B = 0.2358 Pascals * Meters
!      b = -0.625,
!      Mu = 1.256.
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
!    Output, real ( kind = 8 ) SIGMA, the surface tension,
!    in Pascal * m = Newton / m.
!
  implicit none

  real ( kind = 8 ), parameter :: b_cap = 0.2358D+00
  real ( kind = 8 ), parameter :: b_small = -0.625D+00
  real ( kind = 8 ), parameter :: mu = 1.256D+00
  real ( kind = 8 ) sigma
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_star = 647.15D+00
  real ( kind = 8 ) term
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'SURTEN - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if

  term = ( t_star - t ) / t_star
  sigma = b_cap * term**mu * ( 1.0D+00 + b_small * term )
!
!  Need this conversion to match the table, but justification is there none.
!
  sigma = 1000.0D+00 * sigma

  return
end
