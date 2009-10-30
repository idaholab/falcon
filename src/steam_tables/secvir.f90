subroutine secvir ( t, vir )

!*****************************************************************************80
!
!! SECVIR calculates the second virial coefficient at a given temperature.
!
!  Discussion:
!
!    The second virial coefficient VIR appears in the first correction term
!    to the ideal gas equation of state:
!
!      P = R * T / volume + VIR / volume**2 + ...
!
!  Modified:
!
!    28 November 1998
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
!    Output, real ( kind = 8 ) VIR, the second virial coefficient, in CM3/G.
!
  implicit none

  real ( kind = 8 ) b1
  real ( kind = 8 ) b1t
  real ( kind = 8 ) b1tt
  real ( kind = 8 ) b2
  real ( kind = 8 ) b2t
  real ( kind = 8 ) b2tt
  real ( kind = 8 ), parameter, dimension ( 5 ) :: g = (/ &
    -0.53062968529023D+03,  0.22744901424408D+04, -0.26627944829770D+04, &
     0.78779333020687D+03, -0.69830527374994D+02 /)
  real ( kind = 8 ) gascon
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_ref = 647.073D+00
  real ( kind = 8 ) v
  real ( kind = 8 ) vir
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'SECVIR - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if

  call bb ( t, b1, b2, b1t, b2t, b1tt, b2tt )

  v = t_ref / t

  vir = b2 + ( &
      v    * ( g(1) &
    + v    * ( g(2) &
    + v    * ( g(3) &
    + v    * ( g(4) &
    + v**2 *   g(5) ))))) &
    / ( gascon ( ) * t )

  return
end
