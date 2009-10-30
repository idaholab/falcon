subroutine thercon ( t, rho, lambda )

!*****************************************************************************80
!
!! THERCON calculates the thermal conductivity for given temperature and density.
!
!  Modified:
!
!    20 November 1998
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
!    Output, real ( kind = 8 ) LAMBDA, the thermal conductivity,
!    in mW/(m degrees Kelvin).
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ), parameter, dimension ( 0:3 ) :: acof = (/ &
    2.02223D+00, 14.11166D+00, 5.25597D+00, -2.01870D+00 /)
  real ( kind = 8 ), parameter :: a_con = 18.66D+00
  real ( kind = 8 ) b(0:4,0:5)
  real ( kind = 8 ), parameter :: b_con = 1.00D+00
  real ( kind = 8 ), parameter :: c_con = 3.7711D-08
  real ( kind = 8 ) chi
  real ( kind = 8 ) cjth
  real ( kind = 8 ) cjtt
  real ( kind = 8 ) cp
  real ( kind = 8 ) cv
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdr2
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) eta
  real ( kind = 8 ) g
  real ( kind = 8 ) h
  integer i
  integer j
  real ( kind = 8 ) lambda
  real ( kind = 8 ) lambda0
  real ( kind = 8 ) lambda_del
  real ( kind = 8 ), parameter :: omega = 0.4678D+00
  real ( kind = 8 ) p
  real ( kind = 8 ), parameter :: p_ref = 22.115D+00
  real ( kind = 8 ) power
  real ( kind = 8 ) rho
  real ( kind = 8 ), parameter :: rho_ref = 317.763D+00
  real ( kind = 8 ) rho2
  real ( kind = 8 ) s
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_ref = 647.27D+00
  real ( kind = 8 ) total
  real ( kind = 8 ) u

  data b / &
    1.3293046D+00,    1.7018363D+00,   5.2246158D+00,  &
    8.7127675D+00, -1.8525999D+00, &
   -0.40452437D+00,  -2.2156845D+00, -10.124111D+00,   &
   -9.5000611D+00,  0.93404690D+00, &
    0.24409490D+00,   1.6511057D+00,   4.9874687D+00, &
    4.3786606D+00,  0.0D+00, &
    0.018660751D+00, -0.76736002D+00, -0.27297694D+00, &
    -0.91783782D+00, 0.0D+00, &
    -0.12961068D+00,  0.37283344D+00, -0.43083393D+00,  &
     0.0D+00,        0.0D+00, &
    0.044809953D+00, -0.11203160D+00,  0.13333849D+00,  &
    0.0D+00,        0.0D+00 /
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'THERCON - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was T = ', t
    stop
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'THERCON - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  Input value was RHO = ', rho
    stop
  end if
!
!  Compute DPDR, DPDT, ETA.
!
  call therm ( t, rho, a, cjth, cjtt, cp, cv, dpdr, dpdt, g, h, p, s, u )

  call viscosity ( t, rho, eta )
!
!  Convert RHO from G/CM3 to kg/M3,
!  Convert DPDR from ? to ?.
!
  rho2 = 1000.0D+00 * rho
  dpdr2 = dpdr / 1000.0D+00
!
!  Compute LAMBDA0.
!
  total = 0.0D+00
  do i = 0, 3
    total = total + acof(i) * ( t_ref / t )**i
  end do

  lambda0 = sqrt ( t / t_ref ) / total
!
!  Compute CHI.
!
  chi = rho2 * p_ref / ( rho_ref**2 * dpdr2 )
!
!  Compute delta_Lambda
!
  power = - a_con * ( ( t_ref - t ) / t )**2 - b_con * ( ( rho2 - rho_ref ) &
    / rho_ref )**4

  lambda_del = ( c_con / eta ) * ( ( t * rho_ref ) / ( t_ref * rho ) )**2 &
    * ( t_ref / p_ref )**2 * dpdt**2 * chi**omega * sqrt ( rho2 / rho_ref ) &
    * exp ( power )
!
!  Compute LAMBDA.
!
  total = 0.0D+00
  do i = 0, 4
    do j = 0, 5
      total = total + b(i,j) * ( ( t_ref - t ) / t )**i * &
        ( ( rho2 - rho_ref ) / rho_ref )**j
    end do
  end do

  lambda = lambda0 * exp ( ( rho2 / rho_ref ) * total ) + lambda_del
!
!  Temporary fix.
!
  lambda = 1000.0D+00 * lambda

  return
end
