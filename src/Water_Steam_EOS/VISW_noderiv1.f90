SUBROUTINE visw_noderiv1 (T, P, VW)

    implicit none
    
    real*8, intent(in) :: T, P !give it temperature and pressure
    real*8, intent(out) :: VW !return viscosity

    real*8 :: EX, PHI, AM, pwr
    real*8 :: PS
    
   
     CALL PSAT_new_rkp (T,PS)  !get p_sat from subroutine
  
    EX  = 247.8d0/(T+133.15d0)
    PHI = 1.0467d0*(T-31.85d0)
    AM  = 1.d0+PHI*(P-PS)*1.d-11
    pwr = 10.d0**EX
    VW  = 1.d-7*AM*241.4d0*pwr

 
  end subroutine visw_noderiv1





