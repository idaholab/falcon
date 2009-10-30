function gascon ( )

!*****************************************************************************80
!
!! GASCON returns the value of the specific gas constant.
!
!  Note:
!
!    The specific gas constant R is related to the universal gas
!    constant R-bar = 8.31441 J/(mol degrees Kelvin) by the molar mass
!    M = 18.0152 g/mol:
!
!      R = R-bar / M.
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
!    Output, real ( kind = 8 ) GASCON, the value of the specific gas
!    constant, in J/(g degrees Kelvin).
!
  implicit none

  real ( kind = 8 ) gascon

  gascon = 0.461522D+00

  return
end
