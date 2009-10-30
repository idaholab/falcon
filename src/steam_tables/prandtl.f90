subroutine prandtl ( t, p, pr )

!*****************************************************************************80
!
!! PRANDTL computes the Prandtl number.
!
!  Discussion:
!
!    This routine was NOT working properly for large pressures,
!    because the routine CORR was changing the input value of P.
!
!  Modified:
!
!    17 November 1998
!
!  Reference:
!
!    Lester Haar, John Gallagher and George Kell,
!    NBS/NRC Steam Tables:
!    Thermodynamic and Transport Properties and Computer Programs
!    for Vapor and Liquid States of Water in SI Units,
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
!    Output, real ( kind = 8 ) PR, the Prandtl number, dimensionless.
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ) cjth
  real ( kind = 8 ) cjtt
  real ( kind = 8 ) cp
  real ( kind = 8 ) cv
  logical, parameter :: debug = .false.
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) eta
  real ( kind = 8 ) g
  real ( kind = 8 ) h
  real ( kind = 8 ) lambda
  real ( kind = 8 ) p
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ) rhol
  real ( kind = 8 ) rho_start
  real ( kind = 8 ) rhov
  real ( kind = 8 ) s
  real ( kind = 8 ) t
  real ( kind = 8 ) t2
  real ( kind = 8 ) u
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'PRANDTL - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative pressures.
!
  if ( p <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'PRANDTL - Fatal error!'
    write ( *, '(a)' ) '  The input pressure P must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was P = ', p
    stop
  end if
!
!  Compute the density.
!
  if ( debug ) then
    write ( *, * ) 'PRANDTL - Call TSAT, with P = ', p
  end if

  rhol = 0.0D+00
  rhov = 0.0D+00

  call tsat ( p, t2, rhol, rhov )

  if ( debug ) then
    write ( *, * ) 'PRANDTL - T2 = ', t2
  end if

  if ( t < t2 ) then
    rho_start = 1.9D+00
  else
    rho_start = 0.01D+00
  end if

  call dense ( p, t, rho_start, rho, dpdr )

  if ( debug ) then
    write ( *, * ) 'PRANDTL - RHO = ', rho
  end if
!
!  Now from T and RHO, compute CP, ETA and LAMBDA.
!
  call therm ( t, rho, a, cjth, cjtt, cp, cv, dpdr, dpdt, g, h, p, s, u )

  call viscosity ( t, rho, eta )

  call thercon ( t, rho, lambda )

  if ( debug ) then
    write ( *, '(7f10.4)' ) t, p, rho, eta, cp, lambda, pr
  end if

  pr = eta * cp / lambda

  return
end
