subroutine dense ( p, t, rho_start, rho, dpdr )

!*****************************************************************************80
!
!! DENSE computes the density for a given pressure and temperature.
!
!  Discussion:
!
!    The use of the variable RHO_START for two opposing purposes is
!    poor practice and will be corrected one of these days.  Meanwhile,
!    the algorithm's behavior, particularly in the two-phase region,
!    is very suspect.
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
!    Input, real ( kind = 8 ) P, the pressure, in MegaPascals.
!
!    Input, real ( kind = 8 ) T, the temperature, in degrees Kelvin.
!
!    Input, real ( kind = 8 ) RHO_START, an initial guess for the density,
!    in G/CM3.  The value of RHO_START also signals whether a vapor or liquid
!    calculation is to be done.  If DPDR is computed negative, then for
!    0.2967 <= RHO_START, liquid is assumed, otherwise gas.
!
!    Output, real ( kind = 8 ) RHO, the density for the given
!    pressure and temperature, in G/CM3.
!
!    Output, real ( kind = 8 ) DPDR, the partial derivative
!    dP(T,RHO)/dRHO, with T held fixed, in (MegaPascals CM3)/G.
!
  implicit none

  real ( kind = 8 ) ab
  real ( kind = 8 ) ar
  real ( kind = 8 ) cvb
  real ( kind = 8 ) cvr
  real ( kind = 8 ) dp
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdrb
  real ( kind = 8 ) dpdrr
  real ( kind = 8 ) dpdtb
  real ( kind = 8 ) dpdtr
  real ( kind = 8 ) dpdx
  real ( kind = 8 ) errtol
  real ( kind = 8 ) gb
  real ( kind = 8 ) gr
  real ( kind = 8 ) hb
  real ( kind = 8 ) hr
  integer it
  integer, parameter :: it_max = 50
  real ( kind = 8 ) p
  real ( kind = 8 ) pb
  real ( kind = 8 ) pp
  real ( kind = 8 ) pr
  real ( kind = 8 ) rho
  real ( kind = 8 ), parameter :: rho_max = 1.9D+00
  real ( kind = 8 ), parameter :: rho_min = 1.0D-08
  real ( kind = 8 ) rho_start
  real ( kind = 8 ) sb
  real ( kind = 8 ) sr
  real ( kind = 8 ) t
  real ( kind = 8 ) ub
  real ( kind = 8 ) ur
  real ( kind = 8 ) x

!RKP added for debug
  real ( kind = 8 ) p_print
  real ( kind = 8 ) t_print
  real ( kind = 8 ) rho_print

!WRITE(*,*) p, t
!RKP Test, ease errtol by order of magnitude
!  errtol = sqrt ( epsilon ( errtol ) )
  errtol = sqrt ( epsilon ( errtol ) ) * 10.
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'DENSE - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if

!  Refuse to handle zero or negative pressures.
!
  if ( p <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'DENSE - Fatal error!'
    write ( *, '(a)' ) '  The input pressure P must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was P = ', p
    stop
  end if

  rho = rho_start
  rho = max ( rho, rho_min )
  rho = min ( rho, rho_max )
  do it = 1, it_max

    call resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )

    call base ( t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub )

    pp = pb + pr
    dpdr = dpdrb + dpdrr
!
!  Check for negative DP/DRho, which characterizes the two-phase region.
!
    if ( dpdr <= 0.0D+00 ) then

      if ( 0.2967D+00 <= rho_start ) then
        rho = rho * 1.02D+00
      else
        rho = rho * 0.98D+00
      end if

      if ( it <= 10 ) then
        cycle
      end if

    end if

    dpdx = 1.1D+00 * dpdr
    dpdx = max ( dpdx, 0.01D+00 )

    dp = abs ( 1.0D+00 - pp / p )

    if ( dp <= errtol .or. &
       ( 0.3D+00 < rho .and. dp <= errtol ) .or. &
       ( 0.7D+00 < rho .and. dp <= 10.0D+00 * errtol ) ) then
      return
    end if

    x = ( p - pp ) / dpdx
    if ( 0.1D+00 < abs ( x ) ) then
      x = x * 0.1D+00 / abs ( x )
    end if

    rho = rho + x

    rho = max ( rho, rho_min )
    rho = min ( rho, rho_max )

  end do

! RKP edit print line below
p_print = p *1.0e6
t_print = t - 273.15
rho_print = rho * 1000.

!  write ( *, '(a)' ) ' '
  write ( *, '(a,3f12.4)' ) 'dense.f90 - Non-Convergence Warning:', rho_print, p_print, t_print
!  write ( *, '(a)' ) '  The iteration did not converge.'
!  write ( *, '(a,i6)' ) '  Number of iterations was ', it_max
!  write ( *, '(a,3f12.4)' ) '  Last iterate was ', rho_print, p_print, t_print

  return
end
