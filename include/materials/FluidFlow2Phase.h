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
  
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _rho_s;
  MaterialProperty<Real> & _rho_mix;
  MaterialProperty<Real> & _c_f;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _mu_s;
  MaterialProperty<Real> & _rel_perm_w;
  MaterialProperty<Real> & _rel_perm_s;

  VariableValue & _temp;
//  VariableValue & _temp_old;
  MaterialProperty<Real> & _sat_w;
  MaterialProperty<Real> & _sat_s;
  MaterialProperty<Real> & _dTbydP_H;
  MaterialProperty<Real> & _dTbydH_P;
  MaterialProperty<Real> & _sat_enthalpy_w;
  MaterialProperty<Real> & _sat_enthalpy_s;
  MaterialProperty<Real> & _darcy_params_w;
  MaterialProperty<Real> & _darcy_params_s;
  MaterialProperty<RealGradient> & _darcy_flux_w;
  MaterialProperty<RealGradient> & _darcy_flux_s;
  MaterialProperty<RealGradient> & _pore_velocity_w;
  MaterialProperty<RealGradient> & _pore_velocity_s;
  
private:

  bool _has_pressure;
  bool _has_enthalpy;
  VariableValue & _pressure;
  VariableGradient & _grad_p;
  VariableValue & _enthalpy;

};

#endif //FLUIDFLOW2PHASE_H
