subroutine tsat_est ( p, t )

!*****************************************************************************80
!
!! TSAT_EST makes a rough estimate of the saturation temperature.
!
!  Discussion:
!
!    The saturation temperature is also called the vapor temperature.
!
!  Modified:
!
!    02 February 2002
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
!    Input, real ( kind = 8 ) P, the pressure, in MegaPascals.  The tabulated
!    range for P is
!      0.00061173 MegaPascals <= P <= P_CRIT = 22.055 MegaPascals.
!    The input value of P must be positive.
!
!    Output, real ( kind = 8 ) T, the saturation temperature,
!    in degrees Kelvin.  This value will always be in the range
!    [ 273.15, 647.126 ].
!
  implicit none

  integer, parameter :: npol = 4

  real ( kind = 8 ), parameter, dimension ( 0:npol ) :: c = (/ &
    372.83D+00, 27.7589D+00, 2.3819D+00, 0.24834D+00, 0.0193855D+00 /)
  real ( kind = 8 ) dp
  real ( kind = 8 ) dt
  real ( kind = 8 ) errtol
  integer it
  integer, parameter :: it_max = 8
  real ( kind = 8 ) p
  real ( kind = 8 ) pl
  real ( kind = 8 ), parameter :: p_crit = 22.055D+00
  real ( kind = 8 ) pp
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_crit = 647.126D+00
  real ( kind = 8 ), parameter :: t_min = 273.15D+00
  real ( kind = 8 ) t_old

  errtol = sqrt ( epsilon ( errtol ) )
!
!  Refuse to handle zero or negative pressure.
!
  if ( p <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'TSAT_EST - Fatal error!'
    write ( *, '(a)' ) '  The input pressure must be positive!'
    write ( *, '(a,g14.6)' ) '  Your value was P = ', p
    stop
  end if

  if ( p_crit < p ) then
    t = t_crit
    return
  end if
!
!  The initial estimate for T uses a polyonmial in the logarithm of P.
!
  pl = 2.302585D+00 + log ( p )

  call dpoly_val_horner ( npol, c, pl, t )

  t = min ( t, t_crit )
  t = max ( t, t_min )

  dt = 0.0D+00

  do it = 1, it_max

    call psat_est ( t, pp )

    call tdpsdt ( t, dp )

    if ( abs ( p - pp ) < errtol * p ) then
      return
    end if

    dt = t * ( p - pp ) / dp

    t_old = t
    t = t * ( 1.0D+00 + ( p - pp ) / dp )
    t = min ( t, t_crit )
    t = max ( t, t_min )

    if ( abs ( dt ) < errtol * ( abs ( t ) + 1.0D+00 ) ) then
      return
    else if ( abs ( t - t_old ) < errtol ) then
      return
    end if

  end do

  write ( *, '(a)' ) ' '
  write ( *, '(a)' ) 'TSAT_EST - Warning!'
  write ( *, '(a)' ) '  The iteration did not converge.'
  write ( *, '(a,i6)' ) '  Number of iterations was ', it_max
  write ( *, '(a,g14.6)' ) '  Convergence tolerance was ', errtol
  write ( *, '(a,g14.6)' ) '  Last iterate was ', t
  write ( *, '(a,g14.6)' ) '  Last correction was ', dt

  return
end
