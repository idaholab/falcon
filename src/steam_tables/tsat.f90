subroutine tsat ( p, t, rhol, rhov )

!*****************************************************************************80
!
!! TSAT calculates the saturation temperature for a given pressure.
!
!  Discussion:
!
!    The corresponding liquid and vapor densities are also computed.
!    The saturation temperature is also known as the "vapor temperature".
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
!    Input, real ( kind = 8 ) P, the vapor pressure, in MegaPascals.
!
!    Output, real ( kind = 8 ) T, the vapor temperature, in degrees Kelvin.
!
!    Output, real ( kind = 8 ) RHOL, the liquid density, in G/CM3.
!
!    Output, real ( kind = 8 ) RHOV, the vapor density, in G/CM3.
!
  implicit none

  logical, parameter :: debug = .false.
  real ( kind = 8 ) delg
  real ( kind = 8 ) dp
  real ( kind = 8 ) dp2
  real ( kind = 8 ) errtol
  real ( kind = 8 ) gascon
  integer it
  integer, parameter :: it_max = 50
  real ( kind = 8 ) p
  real ( kind = 8 ) p_consistent
  real ( kind = 8 ) rhol
  real ( kind = 8 ) rhov
  real ( kind = 8 ) t
!
!  Initialize output quantities, obliterating any input value.
!
  t = 0.0D+00
  rhol = 0.0D+00
  rhov = 0.0D+00
!
!  Set the error tolerance.
!
  errtol = sqrt ( epsilon ( errtol ) )
!
!  Refuse to handle zero or negative pressure.
!
  if ( p <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'TSAT - Fatal error!'
    write ( *, '(a)' ) '  The input pressure must be positive!'
    write ( *, '(a,g14.6)' ) '  Your value was P = ', p
    stop
  end if
!
!  Estimate the saturation temperature.
!
  call tsat_est ( p, t )

  if ( debug ) then
    write ( *, * ) ' '
    write ( *, * ) 'TSAT:'
    write ( *, '(2g14.6)' ) p, t, rhol, rhov
  end if

  do it = 1, it_max

    call corr ( t, p, p_consistent, rhol, rhov, delg )

    dp = delg * gascon ( ) * t  * rhol * rhov / ( rhol - rhov )

    call tdpsdt ( t, dp2 )

    t = t * ( 1.0D+00 - dp / dp2 )

    if ( debug ) then
      write ( *, '(2g14.6)' ) p, t, rhol, rhov
    end if

    if ( abs ( delg ) < errtol ) then
      return
    end if

  end do

  write ( *, '(a)' ) ' '
  write ( *, '(a)' ) 'TSAT - Warning!'
  write ( *, '(a)' ) '  The iteration did not converge.'
  write ( *, '(a,i6)' ) '  Number of iterations was ', it_max
  write ( *, '(a,g14.6)' ) '  Last iterate was ', t
  write ( *, '(a,g14.6)' ) '  Last DELG was ', delg

  return
end
