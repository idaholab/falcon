subroutine bb ( t, b1, b2, b1t, b2t, b1tt, b2tt )

!*****************************************************************************80
!
!! BB calculates the B's of equations 3 and 4.
!
!  Discussion:
!
!    Here
!
!      b(T) = b1 * ln(T/T0) + sum(j=0,1,3,5) b(j)*(T0/T)**j  (Equation 3)
!
!      Bbar(T) = sum(j=0,1,2,4) B(j)*(T0/T)**j               (Equation 4).
!
!    where
!
!      T0 = 647.073 K
!
!    and the coefficients b(j) and B(j) are
!
!      j    b(j)                         B(j)
!     --    -----------                  ----------
!      0    0.7478629                    1.1278334
!      1   -0.3540782                   -0.5944001
!      2    0                           -5.010996
!      3    0.007159876                  0
!      4    0                            0.63684256
!      5   -0.003528426                  0
!
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
!    Output, real ( kind = 8 ) B1, the coefficient b from equation 3,
!    in CM3/G.
!
!    Output, real ( kind = 8 ) B2, the coefficient Bbar from equation 4,
!    in CM3/G.
!
!    Output, real ( kind = 8 ) B1T, the derivative dB1/dT,
!    in (CM3)/(G Degrees Kelvin).
!
!    Output, real ( kind = 8 ) B2T, the derivative dB2/dT,
!    in (CM3)/(G Degrees Kelvin).
!
!    Output, real ( kind = 8 ) B1TT, the second derivative of B1 with
!    respect to T, in (CM3)/(G (Degrees Kelvin)**2 ).
!
!    Output, real ( kind = 8 ) B2TT, the second derivative of B2 with
!    respect to T, in (CM3)/(G (Degrees Kelvin)**2 ).
!
  implicit none

  real ( kind = 8 ) b1
  real ( kind = 8 ) b1t
  real ( kind = 8 ) b1tt
  real ( kind = 8 ) b2
  real ( kind = 8 ) b2t
  real ( kind = 8 ) b2tt
  real ( kind = 8 ), parameter, dimension ( 10 ) :: bp = (/ &
    0.7478629D+00,   -0.3540782D+00,    0.0D+00,           0.0D+00, &
    0.007159876D+00,  0.0D+00,         -0.003528426D+00,   0.0D+00, &
    0.0D+00,          0.0D+00 /)
  real ( kind = 8 ), parameter, dimension ( 10 ) :: bq = (/ &
    1.1278334D+00,    0.0D+00,         -0.5944001D+00,   -5.010996D+00, &
    0.0D+00,          0.63684256D+00,   0.0D+00,          0.0D+00, &
    0.0D+00,          0.0D+00 /)
  integer i
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_ref = 647.073D+00
  real ( kind = 8 ) v(10)
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'BB - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Set V(I) = ( T_REF / T )**(I-1).
!
  v(1) = 1.0D+00
  do i = 2, 10
    v(i) = v(i-1) * t_ref / t
  end do
!
!  Set B1, B1T, B1TT.
!
  b1 = bp(1) + bp(2) * log ( 1.0D+00 / v(2) )
  b1t = bp(2) * v(2) / t_ref
  b1tt = 0.0D+00
  do i = 3, 10
    b1 = b1 + bp(i) * v(i-1)
    b1t = b1t - dble ( i - 2 ) * bp(i) * v(i-1) / t
    b1tt = b1tt + bp(i) * dble ( i - 2 )**2 * v(i-1) / t**2
  end do

  b1tt = b1tt -  ( b1t / t )
!
!  Set B2, B2T, B2TT.
!
  b2 = bq(1)
  b2t = 0.0D+00
  b2tt = 0.0D+00
  do i = 3, 10
    b2 = b2 + bq(i) * v(i-1)
    b2t = b2t - dble ( i - 2 ) * bq(i) * v(i-1) / t
    b2tt = b2tt + bq(i) * dble ( i - 2 )**2 * v(i-1) / t**2
  end do

  b2tt = b2tt - ( b2t / t )

  return
end
