
namespace SteamTables
{
  extern "C" 
  {

    //subroutine steam_call (p, t, rho, eta)
    double steam_call_( double&, double&, double&, double&);
    
    //subroutine base  (t, rho, ab, cvb, dpdrb, dpdtb, gb, hb, pb, sb, ub)
    double base_( double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&);

    //subroutine bb ( t, b1, b2, b1t, b2t, b1tt, b2tt )
    double bb_( double&, double&, double&, double&, double&, double&, double&);

    //subroutine corr ( t, p, p_consistent, rhol, rhov, delg) 
    double corr_( double&, double&, double&, double&, double&, double&);


    //RKP: How do I pass a "logical" data type around in C++
    //
    //subroutine d_gtle_range_p ( x, a, b, ok )
    //double d_gtle_range_p_(double&, double&, double&, logical&);

    //subroutine d_gtle_range_t ( x, a, b, ok )
    //double d_gtle_range_t_(double&, double&, double&, logical&);

    //subroutine dense ( p, t, rho_start, rho, dpdr )
    double dense_( double&, double&, double&, double&, double&);

    //subroutine dielectric ( t, rho, eps )
    double dielectric_( double&, double&, double&);

    //subroutine dpoly_val_horner ( n, c, x, cx )
    double dpoly_val_horner_( int&, double&, double&, double&);

    //function gascon ( )
    double gascon_();

    //subroutine ideal ( t, ai, cpi, cvi, gi, hi, si, ui )
    double ideal_( double&, double&, double&, double&, double&, double&, double&, double&);

    //subroutine prandtl ( t, p, pr )
    double prandtl_( double&, double&, double&);

    //subroutine psat ( t, p, rhol, rhov )
    double psat_( double&, double&, double&, double&);

    //subroutine psat_est ( t, p )
    double psat_est_( double&, double&);

    //subroutine resid ( t, rho, ar, cvr, dpdrr, dpdtr, gr, hr, pr, sr, ur )
    double resid_( double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&);

    //subroutine secvir ( t, vir )
    double secvir_( double&, double&);

    //subroutine sound ( t, p, c )
    double sound_( double&, double&, double&);

    //subroutine surten ( t, sigma )
    double surten_( double&, double&);

    //subroutine tdpsdt ( t, dp )
    double tdpsdt_( double&, double&);

    //subroutine thercon ( t, rho, lambda )
    double thercon_( double&, double&, double&);

    //subroutine therm ( t, rho, a, cjth, cjtt, cp, cv, dpdr, dpdt, g, h, p, s, u )
    double therm_( double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double&, double);

    //subroutine tsat ( p, t, rhol, rhov )
    double tsat_( double&, double&, double&, double&);

    //subroutine tsat_est ( p, t )
    double tsat_est_( double&, double&);

    //subroutine viscosity ( t, rho, eta )
    double viscosity_( double&, double&, double&);

    //subroutine volume ( t, rho, v, dvdt, dvdr )
    double volume_( double&, double&, double&, double&, double&);
    
    
  }
}
