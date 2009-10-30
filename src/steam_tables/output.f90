subroutine output ( a, c, cjth, cjtt, cp, cv, rho, dpdr, dpdt, dvdr, &
  dvdt, eps, eta, g, h, label, lambda, p, pr, s, sigma, t, u, v, vir )

!***********************************************************************
!
!! OUTPUT prints the values of the thermodynamic quantities.
!
!  Modified:
!
!    26 November 2002
!
!  Author:
!
!    John Burkardt
!
!  Parameters:
!
!    Input, real ( kind = 8 ) A, the Helmholtz function.
!
!    Input, real ( kind = 8 ) C, the speed of sound.
!
!    Input, real ( kind = 8 ) CJTH, the isothermal Joule-Thomson coefficient.
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ) c
  real ( kind = 8 ) cjth
  real ( kind = 8 ) cjtt
  real ( kind = 8 ) cp
  real ( kind = 8 ) cv
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) dvdr
  real ( kind = 8 ) dvdt
  real ( kind = 8 ) eps
  real ( kind = 8 ) eta
  real ( kind = 8 ) g
  real ( kind = 8 ) h
  character ( len = 72 ) label
  real ( kind = 8 ) lambda
  real ( kind = 8 ) p
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ) s
  real ( kind = 8 ) sigma
  real ( kind = 8 ) t
  real ( kind = 8 ) u
  real ( kind = 8 ) v
  real ( kind = 8 ) vir

  write ( *, '(a)' ) ' '
  write ( *, '(a)' ) trim ( label )
  write ( *, '(a)' )' '
  write ( *, '(a,g16.8,a)' ) &
    'A =      ', a,       ' KJ/kg        (Helmholtz function)'
  write ( *, '(a,g16.8,a)' ) &
    'C =      ', c,       ' M/s          (speed of sound)'
  write ( *, '(a,g16.8,a)' ) &
    'CJTH =   ', cjth,    ' K/MPa        (Joule-Thomson coefficient)'
  write ( *, '(a,g16.8,a)' ) &
    'CJTT =   ', cjtt,    ' CM3/g        (Isothermal Joule-Thomson coefficient)'
  write ( *, '(a,g16.8,a)' ) &
    'CP =     ', cp,      ' KJ/(kg K)    (Specific heat at constant pressure)'
  write ( *, '(a,g16.8,a)' ) &
    'CV =     ', cv,      ' KJ/(kg K)    (Specific heat at constant volume)'
  write ( *, '(a,g16.8,a)' ) &
    'RHO =    ', rho,     ' g/CM3        (density)'
  write ( *, '(a,g16.8,a)' ) &
    'DPDR =   ', dpdr,    ' MPa CM3/g    (D Pressure/D Density)'
  write ( *, '(a,g16.8,a)' ) &
    'DPDT =   ', dpdt,    ' MPa/K        (D Pressure/D Temperature)'
  write ( *, '(a,g16.8,a)' ) &
    'DVDR =   ', dvdr,    ' CM6/g2       (d Specific Volume/d Density)'
  write ( *, '(a,g16.8,a)' ) &
    'DVDT =   ', dvdt,    ' CM3/(g K)    (d Specific Volume/ d T)'
  write ( *, '(a,g16.8,a)' ) &
    'EPS =    ', eps,     ' 1            (static dielectric constant)'
  write ( *, '(a,g16.8,a)' ) &
    'ETA =    ', eta,     ' MPa s        (viscosity)'
  write ( *, '(a,g16.8,a)' ) &
    'G =      ', g,       ' KJ/kg        (Gibbs specific energy)'
  write ( *, '(a,g16.8,a)' ) &
    'H =      ', h,       ' KJ/kg        (enthalpy)'
  write ( *, '(a,g16.8,a)' ) &
    'LAMBDA = ', lambda,  ' mW/(m K)     (thermal conductivity)'
  write ( *, '(a,g16.8,a)' ) &
    'P =      ', p,       ' MPa          (pressure)'
  write ( *, '(a,g16.8,a)' ) &
    'PR =     ', pr,      ' 1            (Prandtl number)'
  write ( *, '(a,g16.8,a)' ) &
    'S =      ', s,       ' KJ/(kg K)    (entropy)'
  write ( *, '(a,g16.8,a)' ) &
    'SIGMA =  ', sigma,   ' Pa M         (surface tension)'
  write ( *, '(a,g16.8,a)' ) &
    'T =      ', t,       ' K            (temperature)'
  write ( *, '(a,g16.8,a)' ) &
    'U =      ', u,       ' KJ/kg        (internal energy)'
  write ( *, '(a,g16.8,a)' ) &
    'V =      ', v,       ' CM3/g        (specific volume)'
  write ( *, '(a,g16.8,a)' ) &
    'VIR =    ', vir,     ' CM3/g        (second virial coefficient)'

  return
end
