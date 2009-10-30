subroutine psat ( t, p, rhol, rhov )

!*****************************************************************************80
!
!! PSAT calculates the vapor pressure, and the liquid and vapor densities.
!
!  Discussion:
!
!    These quantities correspond to the input temperature T, corrected
!    so that the Gibbs functions for liquid and vapor phase are
!    equal to within a tolerance.
!
!  Modified:
!
!    04 February 2002
!
!  Reference:
!
!    Lester Haar, John Gallagher and George Kell,
!    NBS/NRC Steam Tables:
!    Thermodynamic and Transport Properties and Computer Programs
!    for Vapor and Liquid States of Water in SI Units,
!    Hemisphere Publishing Corporation, Washington, 1984,
!    TJ270.H3
!
!    C A Meyer, R B McClintock, G J Silvestri, R C Spencer,
!    ASME Steam Tables: Thermodynamic and Transport Properties of Steam,
!    American Society of Mechanical Engineers, 1967.
!
!  Parameters:
!
!    Input, real ( kind = 8 ) T, the vapor temperature, in degrees Kelvin.
!
!    Output, real ( kind = 8 ) P, the vapor pressure, in MegaPascals.
!
!    Output, real ( kind = 8 ) RHOL, the liquid density, in G/CM3.
!
!    Output, real ( kind = 8 ) RHOV, the vapor density, in G/CM3.
!
  implicit none

  real ( kind = 8 ) bot
  real ( kind = 8 ) delg
  real ( kind = 8 ) dp
  real ( kind = 8 ) errtol
  real ( kind = 8 ) gascon
  integer it
  integer, parameter :: it_max = 100
  real ( kind = 8 ) p
  real ( kind = 8 ) p_consistent
  real ( kind = 8 ) p_old
  real ( kind = 8 ) rhol
  real ( kind = 8 ) rhov
  real ( kind = 8 ) t
!
!  Ensure that output quantities are initialized,, obliterating any
!  input values.
!
  p = 0.0D+00
  rhol = 0.0D+00
  rhov = 0.0D+00
!
!  Set the error tolerance.
!
  errtol = 100.0D+00 * sqrt ( epsilon ( errtol ) )
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'PSAT - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Get an estimate for the saturation pressure.
!
  call psat_est ( t, p )

  dp = 0.0D+00

  do it = 1, it_max

    call corr ( t, p, p_consistent, rhol, rhov, delg )

    bot = ( rhol - rhov ) / ( rhol * rhov )

    if ( abs ( bot ) < errtol ) then
      write ( *, * ) 'PSAT - Warning, what is this?'
      bot = sign ( errtol, bot )
    end if

    dp = delg * gascon() * t / bot

    p_old = p
    p = p + dp

    if ( abs ( dp ) <= errtol * ( abs ( p ) + 1.0D+00 ) ) then
      return
    end if

    if ( p <= 0.0D+00 ) then
      write ( *, '(a)' ) ' '
      write ( *, '(a)' ) 'PSAT - Warning!'
      write ( *, '(a)' ) '  The iterates have become nonpositive.'
      write ( *, '(a,i6)' ) '  Iteration number = ', it
      write ( *, '(a,g14.6)' ) '  Last iterate was ', p
      write ( *, '(a,g14.6)' ) '  Previous iterate was ', p_old
      write ( *, '(a,g14.6)' ) '  Last correction was ', dp
      write ( *, '(a)' ) '  Trying to recover...'
      p = 0.5D+00 * p_old
    end if

    if ( p <= 0.0D+00 ) then
      write ( *, '(a)' ) ' '
      write ( *, '(a)' ) 'PSAT - Fatal error!'
      write ( *, '(a)' ) '  The iterates have become nonpositive.'
      write ( *, '(a,i6)' ) '  Iteration number = ', it
      write ( *, '(a,g14.6)' ) '  Last iterate was ', p
      write ( *, '(a,g14.6)' ) '  Previous iterate was ', p_old
      write ( *, '(a,g14.6)' ) '  Last correction was ', dp
      stop
    end if

  end do

  write ( *, '(a)' ) ' '
  write ( *, '(a)' ) 'PSAT - Warning!'
  write ( *, '(a)' ) '  The iteration did not converge.'
  write ( *, '(a,i6)' ) '  The number of iterations was ', it_max
  write ( *, '(a,g14.6)' ) '  Convergence tolerance was ', errtol
  write ( *, '(a,g14.6)' ) '  Last iterate was ', p
  write ( *, '(a,g14.6)' ) '  Last correction was ', dp

  return
end
