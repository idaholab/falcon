subroutine resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )

!*****************************************************************************80
!
!! RESID calculates residual contributions to thermodynamic quantities.
!
!  Discussion:
!
!    The residual function consists of 40 terms.  The first 36 are
!    used in a global least squares fit to experimental data.
!
!    Three terms were added that contribute only in the immediate
!    neighborhood of the critical point
!      (tk-5) <= T <= (tk+5) C
!      0.20   <= rho <= 0.44 g/cm3,
!
!    A single term was added for the region of high pressure and
!    low temperature: T < 75 C, 300 MPa < P.
!
!    Except in these limited regions, the residual function is
!    given by the first 36 terms.  The equation is
!
!      A(residual)(rho,T)=
!        sum(i=1 to 36) (g(i)/k(i)) * (T0/T)**(l(i)) (1-exp(-rho))**(k(i))
!      + sum(i=37 to 40) g(i)*delta(i)**(k(i))
!        * exp(-alpha(i)*delta(i)**(k(i)) - beta(i)*tau(i)**2)
!                                                     (Equation 5)
!
!    where
!
!      g(i) are coefficients determined by fits to data,
!      delta(i) are reduced densities (delta(i)=((rho-rho(i))/rho(i))
!      tau(i) are reduced temperatures (tau(i)=((T-tau(i))/tau(i))
!      rho(i) are specified densities.
!      tau(i) are specified temperatures.
!      The k(i) and l(i) are specified integers.
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
!    Input, real ( kind = 8 ) RHO, the density, in G/CM3.
!
!    Output, real ( kind = 8 ) AR, the residual contribution to the
!    Helmholtz function, in KJ/kg.
!
!    Output, real ( kind = 8 ) CVR, the residual contribution to the
!    isochoric (constant volume) heat capacity, in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) DPDRR, the residual contribution to
!    the partial derivative dP(T,RHO)/dRHO, with T held fixed, in
!    (MegaPascals CM3)/G.
!
!    Output, real ( kind = 8 ) DPDTR, the residual contribution to
!    the partial derivative dP(T,RHO)/dT, with RHO held fixed,
!    in MegaPascals/degrees Kelvin.
!
!    Output, real ( kind = 8 ) GR, the residual contribution to the Gibbs
!    function, in KJ/kg.
!
!    Output, real ( kind = 8 ) HR, the residual contribution to the
!    enthalpy, in KJ/kg.
!
!    Output, real ( kind = 8 ) PR, the residual contribution to the pressure,
!    in MegaPascals.
!
!    Output, real ( kind = 8 ) SR, the residual contribution to the entropy,
!    in KJ/(kg degrees Kelvin).
!
!    Output, real ( kind = 8 ) UR, the residual contribution to the
!    internal energy, in KJ/kg.
!
  implicit none

  real ( kind = 8 ), parameter, dimension ( 4 ) :: aad = (/ &
    34.0D+00, 40.0D+00, 30.0D+00, 1050.0D+00 /)
  real ( kind = 8 ), parameter, dimension ( 4 ) :: aat = (/ &
    20000.0D+00, 20000.0D+00, 40000.0D+00, 25.0D+00 /)
  real ( kind = 8 ), parameter, dimension ( 4 ) :: adz = (/ &
    0.319D+00, 0.319D+00, 0.319D+00, 1.55D+00 /)
  real ( kind = 8 ) ar
  real ( kind = 8 ) att
  real ( kind = 8 ), parameter, dimension ( 4 ) ::  atz = (/ &
    640.0D+00, 640.0D+00, 641.6D+00, 270.0D+00 /)
  real ( kind = 8 ) cvr
  real ( kind = 8 ) dadt
  real ( kind = 8 ) ddz
  real ( kind = 8 ) del
  real ( kind = 8 ) dex
  real ( kind = 8 ) dfdt
  real ( kind = 8 ) dpdrr
  real ( kind = 8 ) dpdtr
  real ( kind = 8 ) e
  real ( kind = 8 ) errtol
  real ( kind = 8 ) ex0
  real ( kind = 8 ) ex1
  real ( kind = 8 ) ex2
  real ( kind = 8 ) fct
  real ( kind = 8 ), parameter, dimension ( 40 ) :: g = (/ &
    -530.62968529023D+00,  0.22744901424408D+04, 0.78779333020687D+03, &
    -69.830527374994D+00,  0.17863832875422D+05,-0.39514731563338D+05, &
    0.33803884280753D+05, -0.13855050202703D+05,-0.25637436613260D+06, &
    0.48212575981415D+06, -0.34183016969660D+06, 0.12223156417448D+06, &
    0.11797433655832D+07, -0.21734810110373D+07, 0.10829952168620D+07, &
   -0.25441998064049D+06, -0.31377774947767D+07, 0.52911910757704D+07, &
   -0.13802577177877D+07, -0.25109914369001D+06, 0.46561826115608D+07, &
   -0.72752773275387D+07,  0.41774246148294D+06, 0.14016358244614D+07, &
   -0.31555231392127D+07,  0.47929666384584D+07, 0.40912664781209D+06, &
   -0.13626369388386D+07,  0.69625220862664D+06,-0.10834900096447D+07, &
   -0.22722827401688D+06,  0.38365486000660D+06, 0.68833257944332D+04, &
    0.21757245522644D+05, -0.26627944829770D+04,-0.70730418082074D+05, &
   -0.225D+00, -1.68D+00, 0.055D+00, -93.0D+00 /)
  real ( kind = 8 ) gascon
  real ( kind = 8 ) gr
  real ( kind = 8 ) hr
  integer i
  integer, parameter, dimension ( 40 ) :: ii = (/ &
    0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6, &
    8,8,8,8,2,2,0,4,2,2,2,4 /)
  integer j
  integer, parameter, dimension ( 40 ) :: jj = (/ &
    2,3,5,7,2,3,5,7,2,3,5,7,2,3,5,7,2,3,5,7,2,3,5,7,2,3,5,7,&
    2,3,5,7,1,4,4,4,0,2,0,0 /)
  integer k
  integer l
  integer nc
  real ( kind = 8 ) pr
  real ( kind = 8 ) q10
  real ( kind = 8 ) q20
  real ( kind = 8 ) q2a
  real ( kind = 8 ) q5t
  real ( kind = 8 ) qm
  real ( kind = 8 ) qp
  real ( kind = 8 ) qr(11)
  real ( kind = 8 ) qt(10)
  real ( kind = 8 ) rho
  real ( kind = 8 ) sr
  real ( kind = 8 ), parameter :: s_ref = 7.6180720166752D+00
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_ref = 647.073D+00
  real ( kind = 8 ) tau
  real ( kind = 8 ) tx
  real ( kind = 8 ), parameter :: u_ref = - 4328.4549774261D+00
  real ( kind = 8 ) ur
  real ( kind = 8 ) v

  errtol = sqrt ( epsilon ( errtol ) )
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'RESID - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'RESID - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was RHO = ', rho
    stop
  end if

  nc = 36
  dpdrr = 0.0D+00
  pr = 0.0D+00
  ar = 0.0D+00
  dadt = 0.0D+00
  cvr = 0.0D+00
  dpdtr = 0.0D+00

  ex0 = - rho
  ex0 = max ( ex0, -225.0D+00 )
  ex0 = min ( ex0,  225.0D+00 )
  e = exp ( ex0 )

  q10 = rho * rho * e
  q20 = 1.0D+00 - e

  qr(1) = 0.0D+00
  qr(2) = q10
  do i = 2, 10
    qr(i+1) = qr(i) * q20
  end do

  v = t_ref / t
  qt(1) = t / t_ref
  do i = 2, 10
    qt(i) = qt(i-1) * v
  end do

  do i = 1, nc

    k = ii(i) + 1
    l = jj(i)
    qp = g(i) * qr(k+1) * qt(l+1)
    pr = pr + qp

    dpdrr = dpdrr + ( 2.0D+00 / rho - ( 1.0D+00 - e * dble ( k - 1 ) / &
      ( 1.0D+00 - e ) ) ) * qp

    ar = ar + g(i) * qr(k+2) * qt(l+1) / ( rho**2 * e * dble ( k ) &
      * gascon ( ) * t )

    dfdt = ( 1.0D+00 - e )**k * dble ( 1 - l ) * qt(l+2) / t_ref / dble ( k )

    dadt = dadt + g(i) * dfdt

    dpdtr = dpdtr + g(i) * dfdt * rho**2 * e * dble ( k ) / ( 1.0D+00 - e )

    cvr = cvr + g(i) * dble ( l ) * dfdt / gascon()

  end do

  qp = 0.0D+00
  q2a = 0.0D+00

  do j = 37, 40

    k = ii(j)
    ddz = adz(j-36)
    del = rho / ddz - 1.0D+00

    if ( abs ( del ) < errtol ) then
      del = errtol
    end if

    ex1 = - aad(j-36) * del**k
    ex1 = max ( ex1, -225.0D+00 )
    ex1 = min ( ex1,  225.0D+00 )
    dex = exp ( ex1 ) * del**jj(j)

    att = aat(j-36)
    tx = atz(j-36)
    tau = ( t / tx ) - 1.0D+00

    ex2 = - att * tau * tau

    ex2 = max ( ex2, -225.0D+00 )
    ex2 = min ( ex2,  225.0D+00 )
    q10 = dex * exp ( ex2 )

    qm = dble ( jj(j) ) / del - dble ( k ) * aad(j-36) * del**(k-1)
    fct = qm * rho**2 * q10 / ddz

    q5t = fct * ( 2.0D+00 / rho + qm / ddz ) - ( rho / ddz )**2 * q10 * &
      ( dble ( jj(j) ) / del**2 + dble ( k * ( k - 1 ) ) * aad(j-36) * &
      del**(k-2) )

    dpdrr = dpdrr + q5t * g(j)
    qp = qp + g(j) * fct
    dadt = dadt - 2.0D+00 * g(j) * att * tau * q10 / tx
    dpdtr = dpdtr - 2.0D+00 * g(j) * att * tau * fct / tx

    q2a = q2a + t * g(j) * att * ( 4.0D+00 * ex2 + 2.0D+00 ) * q10 / tx**2

    ar = ar + q10 * g(j) / ( gascon() * t )

  end do

  cvr = cvr + q2a / gascon()
  pr = pr + qp
  sr = - dadt / gascon()
  ur = ar + sr
!
!  Assign dimensions.
!
  ar =  gascon() * t *  ar
  cvr = gascon() *     cvr
  sr =  gascon() *      sr
  ur =  gascon() * t *  ur
!
!  Adjust energies.
!
  ar = ar + gascon ( ) * t * s_ref - gascon ( ) * u_ref
  sr = sr - gascon ( ) * s_ref
  ur = ur - gascon ( ) * u_ref

  gr = ar + pr / rho
  hr = ur + pr / rho

  return
end
