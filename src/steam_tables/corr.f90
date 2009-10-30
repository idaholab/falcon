subroutine corr ( t, p, p_consistent, rhol, rhov, delg )

!*****************************************************************************80
!
!! CORR evaluates an adjustment to the Gibbs function.
!
!  Discussion:
!
!    CORR is given T and P at or near the vapor pressure and evaluates
!    the corresponding liquid and vapor densities, and the residual
!    function DELG = (GL-GV)/(R*T) where GL and GV are the Gibbs functions
!    for the liquid and vapor phases, respectively.
!
!    These quantities are used to calculate a correction to the vapor
!    pressure or the vapor temperature.
!
!    The states corresponding to the coexisting phases of liquid
!    and vapor for the temperature range from the triple point
!    to within 0.5 C of the critical point 0.01 <= t <= tk-0.5 C
!    have been determined in exact accord with the Gibbs condition
!    of phase equilibrium: DELG = G(g)-G(l) = 0, P, t constant,
!    where G(g) and G(l) are the values of the Gibbs function
!    for saturated gas and liquid respectively.
!
!    For the region (tk-t)<=0.5 C, an exact solution for the
!    Helmholtz function yields values of density for the saturated
!    liquid that are shifted to lower values.  Also, the isotherms
!    in the pressure-density plane and the Gibbs function-density
!    plane are nearly flat, so that it is difficult to obtain
!    solutions.  As an alternative to exact solution, the power
!    law equation is used to define states:
!
!      rho(gas) = 0.322 - 0.657 * (1 - T/647.126)**0.325   (g/cm3).
!      rho(liq) = 0.322 + 0.657 * (1 - T/647.126)**0.325   (g/cm3).
!
!    In a poor instance of programming, the input pressure was
!    originally overwritten on output by a value consistent with
!    the computed densities.  This causes no end of misunderstandings,
!    since other routines expect the value of pressure to be input
!    only.  The code is now revised so that there is an input P
!    and an output P.  In a huff, JVB 05 February 2002.
!
!  Modified:
!
!    05 February 2002
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
!    Input, real ( kind = 8 ) T, the vapor temperature, in degrees Kelvin.
!
!    Input, real ( kind = 8 ) P, the vapor pressure, in MegaPascals.
!
!    Output, real ( kind = 8 ) P_CONSISTENT, the vapor pressure, in MegaPascals,
!    consistent with RHOL and RHOV.  This is equal to the input value of
!    P unless 646.3 <= T.
!
!    Input/output, real ( kind = 8 ) RHOL, the liquid density, in G/CM3.
!    On input, if RHOL is positive, it is used as an initial
!    estimate for the iteration.
!
!    Input/output, real ( kind = 8 ) RHOV, the vapor density, in G/CM3.
!    On input, if RHOV is positive, it is used as an initial
!    estimate for the iteration.
!
!    Output, real ( kind = 8 ) DELG, the residual function (GL-GV)/(R*T),
!    where GL is the liquid Gibbs function, GV the vapor Gibbs function,
!    dimensionless.  If 646.3 < T, DELG is 0.
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ) ab
  real ( kind = 8 ) ar
  real ( kind = 8 ) cd
  real ( kind = 8 ) cjth
  real ( kind = 8 ) cjtt
  real ( kind = 8 ) cp
  real ( kind = 8 ) cv
  real ( kind = 8 ) cvb
  real ( kind = 8 ) cvr
  logical, parameter :: debug = .false.
  real ( kind = 8 ) delg
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdrb
  real ( kind = 8 ) dpdrr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) dpdtb
  real ( kind = 8 ) dpdtr
  real ( kind = 8 ) g
  real ( kind = 8 ) gascon
  real ( kind = 8 ) gb
  real ( kind = 8 ) gl
  real ( kind = 8 ) gr
  real ( kind = 8 ) gv
  real ( kind = 8 ) h
  real ( kind = 8 ) hb
  real ( kind = 8 ) hr
  real ( kind = 8 ) p
  real ( kind = 8 ) p_consistent
  real ( kind = 8 ), parameter :: p_crit = 22.055D+00
  real ( kind = 8 ) pb
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ) rhol
  real ( kind = 8 ), parameter :: rho_min = 1.0D-08
  real ( kind = 8 ) rhov
  real ( kind = 8 ) rho_start
  real ( kind = 8 ) s
  real ( kind = 8 ) sb
  real ( kind = 8 ) sr
  real ( kind = 8 ) t
  real ( kind = 8 ) tau
  real ( kind = 8 ), parameter :: t_crit = 647.1260000001D+00
  real ( kind = 8 ) u
  real ( kind = 8 ) ub
  real ( kind = 8 ) ur

  p_consistent = p
!
!  Initialize output quantities.
!
  delg = 0.0D+00
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'CORR - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative pressures.
!
  if ( p <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'CORR - Fatal error!'
    write ( *, '(a)' ) '  The input pressure P must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was P = ', p
    stop
  end if

  if ( t <= 646.3D+00 ) then

    if ( rhol <= 0.0D+00 ) then
      rho_start = 1.11D+00 - 0.0004D+00 * t
    else
      rho_start = rhol
    end if

    call dense ( p_consistent, t, rho_start, rho, dpdr )

    call therm ( t, rho, a, cjth, cjtt, cd, cv, dpdr, dpdt, g, h, &
      p_consistent, s, u )

    rhol = rho
    gl = g

    if ( rhov <= 0.0D+00 ) then
      rho_start = p_consistent / ( gascon() * t )
    else
      rho_start = rhov
    end if

    call dense ( p_consistent, t, rho_start, rho, dpdr )

    rho = max ( rho, rho_min )

    call therm ( t, rho, a, cjth, cjtt, cp, cv, dpdr, dpdt, g, h, &
      p_consistent, s, u )

    rhov = rho
    gv = g
    delg = ( gl - gv ) / ( gascon() * t )

    p_consistent = p

    if ( debug ) then
      write ( *, '(a,g14.6)' ) '  CORR - RHOL = ', rhol
      write ( *, '(a,g14.6)' ) '  RHOV = ', rhov
    end if

  else if ( t <= t_crit ) then

    if ( debug ) then
      write ( *, '(a)' ) 'CORR - Twilight zone'
    end if

    delg = 0.0D+00
    tau = 0.657128D+00 * ( 1.0D+00 - t / t_crit )**0.325D+00
    rhol = 0.322D+00 + tau
    rhov = 0.322D+00 - tau
    rho = rhov
    call base ( t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub )
    call resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )
    p_consistent = pb + pr

  else

    if ( debug ) then
      write ( *, '(a)' ) ' '
      write ( *, '(a)' ) 'CORR - Weirdo zone'
    end if

    rhol = 0.322D+00
    rhov = 0.322D+00
    p_consistent = p_crit
    delg = 0.0D+00

  end if

  return
end
