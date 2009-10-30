subroutine viscosity ( t, rho, eta )

!*****************************************************************************80
!
!! VISCOSITY calculates the viscosity for given temperature and density.
!
!  Discussion:
!
!    On 02 February 2002, I discovered that the Haar/Gallagher/Kell
!    reference apparently reversed the sign on the A3 coefficient.
!    That made the results better, but still off.
!
!    Apparently Haar/Gallagher/Kell had a transcription error in
!    the value of B(4,1), which they list as -0.273093, but which
!    should be -0.253093.
!
!    These two corrections courtesy of Meyer/McClintock/Silvestri/Spencer.
!
!    Now the results look proper!  And just 12 years late...
!
!  Modified:
!
!    02 February 2002
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
!    International Association for the Properties of Steam,
!    Release on Dynamic Viscosity of Water Substance,
!    National Bureau of Standards, Washington DC, 1975, revised 1983.
!
!    C A Meyer, R B McClintock, G J Silvestri, R C Spencer,
!    ASME Steam Tables: Thermodynamic and Transport Properties of Steam,
!    American Society of Mechanical Engineers,
!    Fifth Edition, 1983,
!    TJ270.A75.
!
!  Parameters:
!
!    Input, real ( kind = 8 ) T, the temperature, in degrees Kelvin.
!
!    Input, real ( kind = 8 ) RHO, the density, in G/CM3.
!
!    Output, real ( kind = 8 ) ETA, the viscosity, in MegaPascal seconds.
!
  implicit none

  integer, parameter :: npol_t = 3

  real ( kind = 8 ), parameter, dimension ( 0:npol_t ) :: a = (/ &
    0.0181583D+00, 0.0177624D+00, 0.0105287D+00, -0.0036744D+00 /)
  real ( kind = 8 ) arg
  real ( kind = 8 ), dimension(0:5,0:4) :: b = reshape ( (/ &
    0.501938D+00,   0.162888D+00,  -0.130356D+00,  &
    0.907919D+00,  -0.551119D+00,   0.146543D+00,  &
    0.235622D+00,   0.789393D+00,   0.673665D+00,  &
    1.207552D+00,   0.0670665D+00, -0.0843370D+00, &
   -0.274637D+00,  -0.743539D+00,  -0.959456D+00,  &
   -0.687343D+00,  -0.497089D+00,   0.195286D+00,  &
    0.145831D+00,   0.263129D+00,   0.347247D+00,  &
    0.213486D+00,   0.100754D+00,  -0.032932D+00,  &
   -0.0270448D+00, -0.0253093D+00, -0.0267758D+00, &
   -0.0822904D+00,  0.0602253D+00, -0.0202595D+00 /), &
    (/ 6,5 /) )
  logical, parameter :: debug = .false.
  real ( kind = 8 ) eta
  real ( kind = 8 ) eta0
  integer i
  integer j
  real ( kind = 8 ) rho
  real ( kind = 8 ), parameter :: rho_max = 1.050D+00
  real ( kind = 8 ), parameter :: rho_ref = 0.317763D+00
  real ( kind = 8 ) t
  real ( kind = 8 ) total
  real ( kind = 8 ), parameter :: t_max = 800.00D+00
  real ( kind = 8 ), parameter :: t_ref = 647.27D+00
  logical, save :: warning1 = .false.
  logical, save :: warning2 = .false.
!
!  Refuse to handle zero or negative temperatures.
!
  if ( t <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'VISCOSITY - Fatal error!'
    write ( *, '(a)' ) '  The input temperature T must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was T = ', t
    stop
  end if

  if ( t_max < t .and. .not. warning1 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'VISCOSITY - Warning (once only)!'
    write ( *, '(a,g14.6)' ) &
      '  The input temperature T should be no more than ', t_max
    write ( *, '(a,g14.6)' ) '  The input value was T = ', t
    write ( *, '(a)' ) ' '
    warning1 = .true.
  end if
!
!  Refuse to handle zero or negative density.
!
  if ( rho <= 0.0D+00 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'VISCOSITY - Fatal error!'
    write ( *, '(a)' ) '  The input density RHO must be positive.'
    write ( *, '(a,g14.6)' ) '  The input value was RHO = ', rho
    stop
  end if

  if ( rho_max < rho .and. .not. warning2 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'VISCOSITY - Warning (once only)!'
    write ( *, '(a,g14.6)' ) &
      '  The input density RHO should be no more than ', rho_max
    write ( *, '(a,g14.6)' ) '  The input value was RHO = ', rho
    write ( *, '(a)' ) ' '
    warning2 = .true.
  end if
!
!  Compute ETA0.
!
  arg = t_ref / t

  call dpoly_val_horner ( npol_t, a, arg, total )

  eta0 = sqrt ( t / t_ref ) / total
!
!  Compute ETA.
!
  total = 0.0D+00
  do i = 0, 5
    do j = 0, 4
      total = total + b(i,j) * ( ( t_ref - t ) / t )**i * &
        ( ( rho - rho_ref ) / rho_ref )**j
    end do
  end do

  eta = eta0 * exp ( ( rho / rho_ref ) * total )

  return
end
