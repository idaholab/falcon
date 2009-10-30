subroutine volume ( t, rho, v, dvdt, dvdr )

!*****************************************************************************80
!
!! VOLUME computes specific volume derivatives given temperature and density.
!
!  Discussion:
!
!    Because A(rho,T) is everywhere single valued and analytic,
!    we can derive closed form relations for all other properties.
!
!    The independent variables are temperature T and density RHO,
!    and differentiation with respect to one variable is to imply that
!    the other is held at a fixed value.
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
!    Input, real ( kind = 8 ) RHO, the fluid density, in G/CM3.
!
!    Output, real ( kind = 8 ) V, the specific volume, in CM3/G.
!
!    Output, real ( kind = 8 ) DVDT, the partial derivative dV(T,RHO)/dT,
!    where V is the specific volume, in CM3 / (G * degrees Kelvin).
!
!    Output, real ( kind = 8 ) DVDR, the partial derivative dV(T,RHO)/dRHO,
!    where V is the specific volume, in CM3**2 / ( G**2 ).
!
  implicit none

  real ( kind = 8 ) ab
  real ( kind = 8 ) ar
  real ( kind = 8 ) cvb
  real ( kind = 8 ) cvr
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdrb
  real ( kind = 8 ) dpdrr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) dpdtb
  real ( kind = 8 ) dpdtr
  real ( kind = 8 ) dvdr
  real ( kind = 8 ) dvdt
  real ( kind = 8 ) gb
  real ( kind = 8 ) gr
  real ( kind = 8 ) hb
  real ( kind = 8 ) hr
  real ( kind = 8 ) pb
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ) sb
  real ( kind = 8 ) sr
  real ( kind = 8 ) t
  real ( kind = 8 ) ub
  real ( kind = 8 ) ur
  real ( kind = 8 ) v
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'VOLUME - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'VOLUME - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was RHO = ', rho
    stop
  end if

  call resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )

  call base ( t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub )

  dpdr = dpdrb + dpdrr
  dpdt = dpdtb + dpdtr

  dvdt = dpdt / ( dpdr * rho**2 )
!
!  Because V = 1/Rho, dV/dRho = -1/Rho**2
!
  dvdr = - 1.0D+00 / rho**2

  v = 1.0D+00 / rho

  return
end
