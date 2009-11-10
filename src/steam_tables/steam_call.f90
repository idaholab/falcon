subroutine steam_call ( p, t, rho, eta )

!***********************************************************************
! RKP, 10-28-09.  This is a portion of the program 'steam_interact.f90"
! originally coded by John Burkardt in 2002.  I downloaded the code from 
! his website http://people.sc.fsu.edu/~burkardt/f_src/steam/steam.html
! and modified the main program to act as a subroutine for getting the 
! water/steam density and viscosity as a function of pressure and temp.
!
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ) c
  real ( kind = 8 ) cjth
  real ( kind = 8 ) cjtt
  real ( kind = 8 ) cp
  real ( kind = 8 ) cv
  logical ok
  real ( kind = 8 ) dpdr
  real ( kind = 8 ) dpdt
  real ( kind = 8 ) dvdr
  real ( kind = 8 ) dvdt
  real ( kind = 8 ) eps
  real ( kind = 8 ) eta
  real ( kind = 8 ) g
  real ( kind = 8 ) gascon
  real ( kind = 8 ) gl
  real ( kind = 8 ) gv
  real ( kind = 8 ) h
  integer iopt
  integer ios
  integer itmax
  character ( len = 72 ) label
  real ( kind = 8 ) lambda
  real ( kind = 8 ) p
  real ( kind = 8 ) p_new
  real ( kind = 8 ), parameter :: p_crit = 22.055D+00
  real ( kind = 8 ), parameter :: p_max = 22.00D+00
  real ( kind = 8 ), parameter :: p_min = 0.0D00
  real ( kind = 8 ) p_crit_new
  real ( kind = 8 ) p_max_new
  real ( kind = 8 ) p_min_new
  real ( kind = 8 ) pl
  real ( kind = 8 ) pr
  real ( kind = 8 ) ps
  real ( kind = 8 ) pv
  real ( kind = 8 ) rho
  real ( kind = 8 ) rhol
  real ( kind = 8 ), parameter :: rho_max = 1.9D+00
  real ( kind = 8 ), parameter :: rho_min = 0.0D+00
  real ( kind = 8 ) rho_start
  real ( kind = 8 ) rhov
  real ( kind = 8 ) s
  real ( kind = 8 ) sigma
  real ( kind = 8 ) t
  real ( kind = 8 ), parameter :: t_crit = 647.1260000001D+00
  real ( kind = 8 ), parameter :: t_max = 647.0D+00
  real ( kind = 8 ), parameter :: t_min = 0.0D+00
  real ( kind = 8 ) t_new
  real ( kind = 8 ) t_crit_new
  real ( kind = 8 ) t_max_new
  real ( kind = 8 ) t_min_new
  real ( kind = 8 ) u
  real ( kind = 8 ) v
  real ( kind = 8 ) vir

! RKP: the following manual input lnes are retained here for testing outside of the subroutine      
!        write ( *, '(a)' ) 'Enter the pressure in Pa'
!        read ( *, *, iostat = ios ) p


! RKP:  This "p" input is expecting pressure in MPa.  Falcon will provide it in Pa
! will do the conversion here so it is transparent in FALCON.
        p_new = p
        p = p / 1.E6
        p_crit_new = p_crit * 1.E6
        p_max_new = p_max * 1.e6
        p_min_new = p_min 

        call d_gtle_range_p ( p_new, p_min_new, p_max_new, ok )
        if ( ok .eqv. .FALSE.) STOP

! RKP: the following manual input lnes are retained here for testing outside of the subroutine     
!        write ( *, '(a)' ) 'Enter the temperature in degrees Celsius'
!        read ( *, *, iostat = ios ) t

! RKP:  This "t" input is expecting temperature in Kelvin.  Falcon will provide it in deg C
! will do the conversion here so it is transparent in FALCON.
        t_new = t
        t = t + 273.15
        t_crit_new = t_crit - 273.15
        t_max_new = t_max - 273.15
        t_min_new = t_min 
        call d_gtle_range_t ( t_new, t_min_new, t_max_new, ok )
        if ( ok .eqv. .FALSE.) STOP
            
!  Get the density as function of P and T.
      ps = 20000.0D+00
      rhol = 0.0D+00
      rhov = 0.0D+00

      if ( t < t_crit ) then
        call psat ( t, ps, rhol, rhov )
      end if

      if ( p > ps ) then
        rho_start = rhol
      else
        rho_start = p / ( gascon() * t )
      end if

      call dense ( p, t, rho_start, rho, dpdr )
      call viscosity ( t, rho, eta )

! RKP. Here are all the other subroutine calls.  I commented them out on 10-28-09.
! As FALCON evolves we will need to use more of these,but for now will keep it 
! simple.

!  Compute properties as function of T and D.

!      call therm ( t, rho, a, cjth, cjtt, cp, cv, dpdr, dpdt, g, h, p, s, u )
!      call dielectric ( t, rho, eps )
!      call prandtl ( t, p, pr )
!      call secvir ( t, vir )
!      call sound ( t, p, c )
!      call surten ( t, sigma )
!      call thercon ( t, rho, lambda )
!      call volume ( t, rho, v, dvdt, dvdr )

! RKP: FALCON expects the density to be in units of (kg/m^3), and the viscosity to be
!in units of (Pa s).  Will do the conversions here.

rho = rho * 1000. ! this is the densoty in kg/m^3
eta = eta / 1.E6 !this is the viscosity in Pa s
p = p * 1.E6 !convert back to Pa before the subroutine returns the value
t = t - 273.15 !convert back to celsius before returning the valur


! RKP: the following manual output lnes are retained here for testing outside of the subroutine 

! write ( *, '(a,g16.8,a)' ) &
!    'RHO =    ', rho,     ' kg/M3        (density)'
! write ( *, '(a,g16.8,a)' ) &
!    'ETA =    ', eta,     ' Pa s        (viscosity)'

end
