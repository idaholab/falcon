subroutine therm ( t, rho, a, cjth, cjtt, cp, cv, dpdr, dpdt, g, h, p, s, u )

!*****************************************************************************80
!
!! THERM calculates thermodynamic functions given temperature and density.
!
!  Discussion:
!
!    Thermodynamic values were calculated from an analytic equation
!    that approximates the Helmholtz function (specific Helmholtz
!    energy) for ordinary water and steam, of the form A=A(rho,T)
!    where A is the Helmholtz function, rho the density, and T
!    the absolute (thermodynamic) temperature.  Any thermodynamic
!    value for any state, liquid, vapor or metastable, may be
!    calculated by differentiation of this equation in accord with
!    the first and second laws of thermodynamics.
!
!    The International Association for the Properties of Steam
!    has provisionally accepted this formulation for the range
!    273.15 <= T <= 1273.15 degrees Kelvin, where, for 423.15 <= T,
!    the maximum pressure is Pmax = 1500 MPa = 15000 bar, and for
!    273.15 <= T < 423.15, the maximum pressure is
!    Pmax = 100 * (5 + (T-273.15)/15) MPa.
!
!    Close to the critical point, a small region is excluded:
!    Abs(T-Tk) < 1, abs((rho-rhok)/rhok) < 0.3.
!
!    The equation has a wider useful range, namely, fluid states
!    of pure, undissociated water and steam defined by
!    260 <= T <= 2500 K and 0 <= P <= 3000 MPa.
!
!    Thermodynamic property values for specific volume, density,
!    specific internal energy, specific enthalpy, and specific
!    entropy of water and steam were tabulated over the range
!    0 <= t <= 2000 C, 0 <= P <= 3000 MPa.  The reference
!    state is the liquid at the triple point, for which the
!    internal energy and entropy have been assigned the value zero.
!
!    Thermodynamic quantities are determined from the Helmholtz function
!    A(rho,T), which is computed as the sum of three terms:
!
!      A(rho,T) = A(base)(rho,T) + A(residual)(rho,T) + A(ideal)(T)
!                                                       (Equation 1)
!
!    Because A(rho,T) is everywhere single valued and analytic,
!    we can derive closed form relations for all other properties.
!    In the following, unless otherwise indicated, the independent
!    variables are temperature T and density RHO, and differentiation
!    with respect to one variable is to imply that the other is fixed.
!
!    Pressure:                  P       = RHO**2 * dA/dRHO
!    Density derivative:        dP/dRHO = 2*P/RHO + RHO**2 * d2A/dRHO2
!    Temperature derivative:    dP/dT   = RHO**2 * d2A/(dRHO dT)
!    Specific entropy:          S       = - dA/dT
!    Specific internal energy:  U       = A + T*S
!    Specific enthalpy:         H       = U + P/RHO
!    Specific heat capacity
!      at constant volume:      Cv      = - T * d2A/dT2
!    Specific Gibbs function:   G       = A + P/RHO
!    Specific heat capacity
!      at constant pressure:    Cp      = Cv + (T*(dP/dT)**2)/(RHO**2*dP/dRHO)
!    Speed of sound:            Omega   = Sqrt ((Cp/Cv) * dP/dRHO)
!    Second virial coefficient: B       = 1/(2*R*T) * (d2P/dRHO2) (at RHO=0)
!    Isothermal Joule-Thomson
!      coefficient:             DeltaT  = (dH/dP) (fixed T) =
!                                         (1/RHO)-(T*dP/dT)/(RHO**2*dP/dRHO)
!    Joule-Thomson coefficient: Mu      = (dT/dP) (fixed H) = DeltaT/Cp
!    Isentropic temperature-
!      pressure coefficient:    BetaS   = (dT/dP) (fixed S) =
!                                         (DeltaT - 1/RHO)/Cp
!
!  Modified:
!
!    19 November 1998
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
!    Output, real ( kind = 8 ) A, the Helmholtz function, in KJ/kg.
!
!    Output, real ( kind = 8 ) CJTH, the Joule-Thomson coefficient,
!    in K/MegaPascals.
!
!    Output, real ( kind = 8 ) CJTT, the isothermal Joule-Thomson coefficient,
!    in CM3/G.
!
!    Output, real ( kind = 8 ) CP, the isobaric (constant pressure) heat
!    capacity, in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) CV, the isochoric (constant volume) heat capacity,
!    in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) DPDR, the partial derivative
!    dP(T,RHO)/dRHO, with T held fixed, in MegaPascals*CM3/G.
!
!    Output, real ( kind = 8 ) DPDT, the partial derivative
!    dP(T,RHO)/dT, with RHO held fixed, in MegaPascals/degrees Kelvin.
!
!    Output, real ( kind = 8 ) G, the Gibbs free energy, in KJ/kg.
!
!    Output, real ( kind = 8 ) H, the enthalpy, in KJ/kg.
!
!    Output, real ( kind = 8 ) P, the pressure, in MegaPascals.
!
!    Output, real ( kind = 8 ) S, the entropy, in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) U, the internal energy, in KJ/kg.
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ) ab
  real ( kind = 8 ) ai
  real ( kind = 8 ) ar
  real ( kind = 8 ) cjth
  real ( kind = 8 ) cjtt
  real ( kind = 8 ) cp
  real ( kind = 8 ) cpi
  real ( kind = 8 ) cv
  real ( kind = 8 ) cvb
  real ( kind = 8 ) cvi
  real ( kind = 8 ) cvr
  logical, parameter :: debug = .false.
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdrb
  real ( kind = 8 ) dpdrr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) dpdtb
  real ( kind = 8 ) dpdtr
  real ( kind = 8 ) g
  real ( kind = 8 ) gb
  real ( kind = 8 ) gi
  real ( kind = 8 ) gr
  real ( kind = 8 ) h
  real ( kind = 8 ) hb
  real ( kind = 8 ) hi
  real ( kind = 8 ) hr
  real ( kind = 8 ) p
  real ( kind = 8 ) pb
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ) s
  real ( kind = 8 ) sb
  real ( kind = 8 ) si
  real ( kind = 8 ) sr
  real ( kind = 8 ) t
  real ( kind = 8 ) u
  real ( kind = 8 ) ub
  real ( kind = 8 ) ui
  real ( kind = 8 ) ur
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'THERM - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'THERM - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was RHO = ', rho
    stop
  end if

  call ideal ( t, ai, cpi, cvi, gi, hi, si, ui )

  call resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )

  call base ( t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub )

  a =       ab +    ar +  ai
  cv =     cvb +   cvr + cvi

  if ( debug ) then
    write ( *, * ) ' '
    write ( *, * ) 'THERM:'
    write ( *, * ) '  CVB = ', cvb
    write ( *, * ) '  CVR = ', cvr
    write ( *, * ) '  CVI = ', cvi
    write ( *, * ) '  CV  = ', cv
  end if

  dpdr = dpdrb + dpdrr
  dpdt = dpdtb + dpdtr
  p =       pb +    pr
  s =       sb +    sr +  si
  u =       ub +    ur +  ui

  if ( debug ) then
    write ( *, * ) ' '
    write ( *, * ) 'THERM:'
    write ( *, * ) '  UB = ', ub
    write ( *, * ) '  UR = ', ur
    write ( *, * ) '  UI = ', ui
  end if

  g = a + p / rho
  h = u + p / rho
  cp = cv + t * dpdt**2 / ( dpdr * rho**2 )
  cjtt = 1.0D+00 / rho - t * dpdt / ( dpdr * rho**2 )
  cjth = - cjtt / cp

  return
end
