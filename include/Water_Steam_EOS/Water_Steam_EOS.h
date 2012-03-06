/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/


namespace Water_Steam_EOS
{
  extern "C" 
  {

    //subroutine TSAT(p, 100D0, Ts)
    double TSAT_( double&, double&, double&);
    
    //subroutine water_steam_prop_PH(p, h, Ts,T, Sw, Den, dTdp, dTdh, dDendp, dDendh,  ierr, dhwdp, dhsdp, dTdp)
    void water_steam_prop_ph_( double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, 
                               double&, double&, double&, double&, double&, int&, double&, double&,double&);
    // subroutine water_steam_prop_PH_noderiv(p, h, T, Sw, Den,Denw, Dens, hw, hs,visw,viss,ierror)
    void water_steam_prop_ph_noderiv_(double&,double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, int&);
    
    
    //subroutine  wateos_noderiv(T, p, dw, dwmol, hw, energyscale, ierr)
    // extern "C" double watereos_( double&, double&, double&, double&, double&, double&, double&); 
    //void water_eos1_( double&, double&, double&);
    
    //subroutine  wateos_noderiv(T, p, dw)  this one is faster, no derivatives calculated
    void wateos_noderiv1_( double&, double&, double&);
	  
    //subroutine  VISW_noderiv1 (T,P,VW)
    double visw_noderiv1_( double&, double&, double&);
      
    //subroutine  VISW_noderiv1 (rho_s,tc,vs)  
    double viss_noderiv1_( double&, double&, double&);  
    
   //    
  }

}
