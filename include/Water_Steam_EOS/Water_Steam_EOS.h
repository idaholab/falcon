
namespace Water_Steam_EOS
{
  extern "C" 
  {

    //subroutine TSAT(p, 100D0, Ts)
    double TSAT_( double&, double&, double&);
    
    //subroutine wateos_noderiv(Ts, p, dw, dwmol, hw, energyscale, ierr)
    double watereos_noderiv_( double&, double&, double&, double&, double&, double&, double&);

    //subroutine steameos(Ts, p, 0D0, dg, dgmol, dgp,dgt,hg,hgp,hgt, energyscale,ierr)
    double steameos_( double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&);
	  
    //subroutine water_steam_prop_PH(p, h, Ts,T, Sw, Den, dTdp, dTdh, dDendp, dDendh,  ierr)
    double water_steam_prop_PH_( double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&);
								
    //subroutine  wateos_noderiv(T, p, dw, dwmol, hw, energyscale, ierr)
    // extern "C" double watereos_( double&, double&, double&, double&, double&, double&, double&); 
	extern "C" void watereos1_( double&, double&, double&); 
	  
    //subroutine  VISW_FLO (t,dw,vw)
    double visw_flo_( double&, double&, double&);  
  }
}
