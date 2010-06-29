#ifndef FLUIDFLOW2PHASE_H
#define FLUIDFLOW2PHASE_H

#include "PorousMedia.h"


//Forward Declarations
class FluidFlow2Phase;

template<>
InputParameters validParams<FluidFlow2Phase>();

/**
 * Simple material with FluidFlow2Phase properties.
 */
class FluidFlow2Phase : virtual public PorousMedia
{
public:
  FluidFlow2Phase(std::string name,
            MooseSystem & moose_system,
           InputParameters parameters);
  
protected:
  virtual void computeProperties();
  Real _input_rho_w;
  Real _input_rho_s;
  Real _input_c_f;
  Real _input_mu_w;
  Real _input_mu_s;
  Real _input_rel_perm_w;
  Real _input_rel_perm_s;
  
  MooseArray<Real> & _rho_w;
  MooseArray<Real> & _rho_s;
  MooseArray<Real> & _rho_mix;
  MooseArray<Real> & _c_f;
  MooseArray<Real> & _mu_w;
  MooseArray<Real> & _mu_s;
  MooseArray<Real> & _rel_perm_w;
  MooseArray<Real> & _rel_perm_s;

  MooseArray<Real> & _temp;
//  MooseArray<Real> & _temp_old;
  MooseArray<Real> & _sat_w;
  MooseArray<Real> & _sat_s;
  MooseArray<Real> & _dTbydP_H;
  MooseArray<Real> & _dTbydH_P;
  MooseArray<Real> & _sat_enthalpy_w;
  MooseArray<Real> & _sat_enthalpy_s;
  MooseArray<Real> & _darcy_params_w;
  MooseArray<Real> & _darcy_params_s;
  MooseArray<RealGradient> & _darcy_flux_w;
  MooseArray<RealGradient> & _darcy_flux_s;
  MooseArray<RealGradient> & _pore_velocity_w;
  MooseArray<RealGradient> & _pore_velocity_s;
  
private:

  bool _has_pressure;
  bool _has_enthalpy;
  MooseArray<Real> & _pressure;
  MooseArray<RealGradient> & _grad_p;
  MooseArray<Real> & _enthalpy;

};

#endif //FLUIDFLOW2PHASE_H
