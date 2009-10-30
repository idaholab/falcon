subroutine dielectric ( t, rho, eps )

!*****************************************************************************80
!
!! DIELECTRIC returns the static dielectric constant.
!
!  Discussion:
!
!    According to the IAPS, the equation used is valid in the range
!
!      273.15 degrees Kelvin <= T <= 823.15 degrees K
!      0 MegaPascals         <= P <= 500 MegaPascals.
!
!  Modified:
!
!    02 February 2002
!
!  Author:
!
!    John Burkardt
!
!  Reference:
!
!    Lester Haar, John Gallagher and George Kell,
!    NBS/NRC Steam Tables:
!    Thermodynamic and Transport Properties and Computer Programs
!      for Vapor and Liquid States of Water in SI Units,
!    Hemisphere Publishing Corporation, Washington, 1984,
!    TJ270.H3, page 266.
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
!    Output, real ( kind = 8 ) EPS, the dielectric constant, dimensionless.
!
  implicit none

  integer, parameter :: npol_c = 4

  real ( kind = 8 ), parameter, dimension ( 10 ) :: a = (/ &
    7.62571D+00,  244.003D+00,  -140.569D+00,   27.7841D+00, -96.2805D+00, &
   41.7909D+00,   -10.2099D+00,  -45.2059D+00,  84.6395D+00, -35.8644D+00 /)
  real ( kind = 8 ) c(0:npol_c)
  real ( kind = 8 ) eps
  real ( kind = 8 ) rho
  real ( kind = 8 ) t
  real ( kind = 8 ) t_copy
  real ( kind = 8 ), parameter :: t_max = 823.15D+00
  real ( kind = 8 ), parameter :: t_min = 273.15D+00
  real ( kind = 8 ), parameter :: t_ref = 298.15D+00
  real ( kind = 8 ) t_star
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'DIELECTRIC - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'DIELECTRIC - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was RHO = ', rho
    stop
  end if

  t_copy = t
  t_copy = min ( t_copy, t_max )
  t_copy = max ( t_copy, t_min )

  t_star = t_copy / t_ref

  c(0) = 1.0D+00
  c(1) = a(1) / t_star
  c(2) = ( a(2) / t_star ) + a(3) + a(4) * t_star
  c(3) = ( a(5) / t_star ) + a(6) * t_star + a(7) * t_star**2
  c(4) = ( a(8) / t_star**2 ) + ( a(9) / t_star ) + a(10)

  call dpoly_val_horner ( npol_c, c, rho, eps )

  return
end
