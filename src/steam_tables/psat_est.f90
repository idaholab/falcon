subroutine psat_est ( t, p )

!*****************************************************************************80
!
!! PSAT_EST makes a rough estimate of the vapor pressure.
!
!  Discussion:
!
!    The calculation agrees with tabulated data to within
!    0.02% for temperature to within a degree or so of the critical
!    temperature.  The approximate vapor pressure can be refined
!    by imposing the condition that the Gibbs functions of the vapor
!    and liquid phases be equal.
!
!  Modified:
!
!    21 November 1998
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
!    Output, real ( kind = 8 ) P, the vapor pressure, in MegaPascals.
!
  implicit none

  real ( kind = 8 ), parameter, dimension ( 8 ) :: a = (/ &
    -7.8889166D+00,   2.5514255D+00,   -6.716169D+00, 33.239495D+00, &
    -105.38479D+00,   174.35319D+00,  -148.39348D+00, 48.631602D+00 /)
  real ( kind = 8 ) b
  integer i
  real ( kind = 8 ) p
  real ( kind = 8 ) q
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_ref = 647.25D+00
  real ( kind = 8 ) v
  real ( kind = 8 ) w
  real ( kind = 8 ) z
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'PSAT_EST - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if

  if ( t <= 314.0D+00 ) then

    p = 0.1D+00 * exp ( 6.3573118D+00 - 8858.843D+00 / t &
      + 607.56335D+00 * t**( -0.6D+00 ) )

  else

    v = t / t_ref
    w = abs ( 1.0D+00 - v )
    b = 0.0D+00
    do i = 1, 8
      z = i
      b = b + a(i) * w**( ( z + 1.0D+00 ) / 2.0D+00 )
    end do

    q = b / v
    p = 22.093D+00 * exp ( q )

  end if

  return
end
