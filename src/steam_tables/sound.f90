subroutine sound ( t, p, c )

!*****************************************************************************80
!
!! SOUND computes the speed of sound given temperature and pressure.
!
!  Modified:
!
!    22 November 1998
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
!    Input, real ( kind = 8 ) P, the pressure, in MegaPascals.
!
!    Output, real ( kind = 8 ) C, the speed of sound, in m/s.
!
  implicit none

  real ( kind = 8 ) ab
  real ( kind = 8 ) ai
  real ( kind = 8 ) ar
  real ( kind = 8 ) c
  real ( kind = 8 ) cp
  real ( kind = 8 ) cpi
  real ( kind = 8 ) cv
  real ( kind = 8 ) cvb
  real ( kind = 8 ) cvi
  real ( kind = 8 ) cvr
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdrb
  real ( kind = 8 ) dpdrr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) dpdtb
  real ( kind = 8 ) dpdtr
  real ( kind = 8 ) gb
  real ( kind = 8 ) gi
  real ( kind = 8 ) gr
  real ( kind = 8 ) hb
  real ( kind = 8 ) hi
  real ( kind = 8 ) hr
  real ( kind = 8 ) p
  real ( kind = 8 ) pb
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ) rhol
  real ( kind = 8 ) rho_start
  real ( kind = 8 ) rhov
  real ( kind = 8 ) sb
  real ( kind = 8 ) si
  real ( kind = 8 ) sr
  real ( kind = 8 ) t
  real ( kind = 8 ) t2
  real ( kind = 8 ) ub
  real ( kind = 8 ) ui
  real ( kind = 8 ) ur
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'SOUND - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative pressures.
!
  if ( p <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'SOUND - Fatal error!'
    write ( *, '(a)' ) '  The input pressure P must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was P = ', p
    stop
  end if
!
!  For the given pressure, compute the saturation temperature.
!
  rhol = 0.0D+00
  rhov = 0.0D+00

  call tsat ( p, t2, rhol, rhov )
!
!  Depending on whether the temperature is above or below the
!  saturation temperature, we expect to compute the density of
!  a liquid or vapor.
!
  if ( t < t2 ) then
    rho_start = 1.9D+00
  else
    rho_start = 0.01D+00
  end if

  call dense ( p, t, rho_start, rho, dpdr )
!
!  From T and RHO, compute the thermodynamic properties.
!
  call ideal ( t, ai, cpi, cvi, gi, hi, si, ui )

  call resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )

  call base ( t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub )

  cv =     cvb +   cvr + cvi
  dpdr = dpdrb + dpdrr
  dpdt = dpdtb + dpdtr

  cp = cv + t * dpdt**2 / ( dpdr * rho**2 )

  c = sqrt ( 1000.0D+00 * cp * dpdr / cv )

  return
end
