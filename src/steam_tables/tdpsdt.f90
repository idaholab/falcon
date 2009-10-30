subroutine tdpsdt ( t, dp )

!*****************************************************************************80
!
!! TDPSDT computes the quantity T * dP(Sat)/dT.
!
!  Discussion:
!
!    Here T is the temperature and P(Sat) is the vapor pressure.
!    It is used by TSAT_EST and TSAT.
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
!    Output, real ( kind = 8 ) DP, the value T*(dP(Sat)/dT),
!    in MegaPascals.
!
  implicit none

  real ( kind = 8 ), parameter, dimension ( 8 ) :: a = (/ &
      -7.8889166D+00,   2.5514255D+00,   -6.716169D+00, 33.239495D+00, &
    -105.38479D+00,   174.35319D+00,   -148.39348D+00,  48.631602D+00 /)
  real ( kind = 8 ) b
  real ( kind = 8 ) c
  real ( kind = 8 ) dp
  integer i
  real ( kind = 8 ) q
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_ref = 647.25D+00
  real ( kind = 8 ) v
  real ( kind = 8 ) w
  real ( kind = 8 ) y
  real ( kind = 8 ) z
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'TDPSDT - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if

  v = t / t_ref
  w = 1.0D+00 - v
  b = 0.0D+00
  c = 0.0D+00
  do i = 1, 8
    z = dble ( i + 1 ) / 2.0D+00
    y = a(i) * w**z
    c = c + ( y / w ) * ( 0.5D+00 - 0.5D+00 * dble ( i ) - 1.0D+00 / v )
    b = b + y
  end do

  q = b / v
  dp = 22.093D+00 * exp ( q ) * c

  return
end
