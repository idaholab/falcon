subroutine ideal ( t, ai, cpi, cvi, gi, hi, si, ui )

!*****************************************************************************80
!
!! IDEAL computes ideal gas thermodynamic properties of water.
!
!  Discussion:
!
!    Values for thermodynamic properties of water in the ideal
!    gas state were reported by Woolley.  The formula for the ideal gas
!    term of the Helmholtz function approximates a term by term summation of
!    contributions from each of the rotation and vibration states.
!    The formula, equation #6 in the reference, is:
!
!    A(ideal)(T) = -R * T * ( 1 + ( C(1)/Tr + C(2) ) * ln(Tr)
!      + Sum ( 3 <= I <= 18) C(I) * Tr**(I-6)
!
!    where Tr=T/100 K.  The C(i) are tabulated coefficients.  Equation
!    6 can be used for temperatures below 3000 K, and is accurate to
!    within the tolerance of the gas constant for 50<=T<=2000 K.
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
!  Parameters:
!
!    Input, real ( kind = 8 ) T, the temperature, in degrees Kelvin.
!
!    Output, real ( kind = 8 ) AI, the Helmholtz function, in KJ/kg.
!
!    Output, real ( kind = 8 ) CPI, the heat capacity at constant pressure,
!    in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) CVI, the heat capacity at constant volume,
!    in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) GI, the Gibbs free energy, in KJ/kg.
!
!    Output, real ( kind = 8 ) HI, the enthalpy, in KJ/kg.
!
!    Output, real ( kind = 8 ) SI, the entropy, in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) UI, the internal energy, in KJ/kg.
!
  implicit none

  real ( kind = 8 ) ai
  real ( kind = 8 ), parameter, dimension ( 18 ) :: c = (/ &
   19.730271018D+00,      20.9662681977D+00,     -0.483429455355D+00, &
    6.05743189245D+00,    22.56023885D+00,       -9.87532442D+00, &
   -4.3135538513D+00,      0.458155781D+00,      -0.047754901883D+00, &
    0.0041238460633D+00,  -0.00027929052852D+00,  0.14481695261D-04, &
   -0.56473658748D-06,     0.16200446D-07,       -0.3303822796D-09, &
    0.451916067368D-11,   -0.370734122708D-13,    0.137546068238D-15 /)
  real ( kind = 8 ) cpi
  real ( kind = 8 ) cvi
  real ( kind = 8 ) gascon
  real ( kind = 8 ) gi
  real ( kind = 8 ) hi
  integer i
  real ( kind = 8 ) si
  real ( kind = 8 ) t
  real ( kind = 8 ) tt
  real ( kind = 8 ) ui
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'IDEAL - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if

  tt = t / 100.0D+00

  gi = - ( c(1) / tt + c(2) ) * log ( tt )
  do i = 3, 18
    gi = gi - c(i) * tt**(i-6)
  end do

  hi = c(2) + c(1) * ( 1.0D+00 - log ( tt ) ) / tt
  do i = 3, 18
    hi = hi + dble ( i - 6 ) * c(i) * tt**(i-6)
  end do

  cpi = c(2) - c(1) / tt
  do i = 3, 18
    cpi = cpi + dble ( ( i - 6 ) * ( i - 5 ) ) * c(i) * tt**(i-6)
  end do

  ai = gi - 1.0D+00
  ui = hi - 1.0D+00
  cvi = cpi - 1.0D+00
  si = hi - gi
!
!  Assign dimensions.
!
  ai =  gascon() * t * ai
  cpi = gascon()     * cpi
  cvi = gascon()     * cvi
  gi =  gascon() * t * gi
  hi =  gascon() * t * hi
  si =  gascon()     * si
  ui =  gascon() * t * ui

  return
end
