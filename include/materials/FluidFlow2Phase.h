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
  FluidFlow2Phase(const std::string & name,
            MooseSystem & moose_system,
                  InputParameters parameters);
  
protected:
  virtual void computeProperties();

  VariableGradient & _grad_p;
  VariableValue & _pressure;
  VariableValue & _pressure_old;
  VariableValue & _enthalpy;
  VariableValue & _enthalpy_old;

  Real _input_rho_w;
  Real _input_rho_s;
  Real _input_c_f;
  Real _input_mu_w;
  Real _input_mu_s;
  
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _rho_s;
  MaterialProperty<Real> & _c_f;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _mu_s;
  MaterialProperty<Real> & _rel_perm_w;
  MaterialProperty<Real> & _rel_perm_s;

  MaterialProperty<Real> & _temp;
  MaterialProperty<Real> & _temp_old;
  MaterialProperty<Real> & _dTbydP_H;
  MaterialProperty<Real> & _dTbydH_P;
  
  MaterialProperty<Real> & _rho;
  MaterialProperty<Real> & _rho_old;
  MaterialProperty<Real> & _drhobydP_H;
  MaterialProperty<Real> & _drhobydH_P;

  MaterialProperty<Real> & _sat_w;
  MaterialProperty<Real> & _sat_s;
  MaterialProperty<Real> & _Hw;
  MaterialProperty<Real> & _Hs;
  MaterialProperty<RealGradient> & _GHw;
  MaterialProperty<RealGradient> & _GHs;
  MaterialProperty<Real> & _darcy_params_w;
  MaterialProperty<Real> & _darcy_params_s;
  MaterialProperty<RealGradient> & _darcy_flux_w;
  MaterialProperty<RealGradient> & _darcy_flux_s;

  Real E3;
  Real E6;
  Real E5;
  Real E7;

// coefficients related to rho_w empirical equation     
  Real a1;
  Real a2;
  Real a3;
  Real a4;
  Real a5;
  Real a6;

// coefficients related to rho_s empirical equation
  Real b1;
  Real b2;
  Real b3;
  Real b4;
  Real b5;

// coefficients related to Hs empirical equation
  Real c1;
  Real c2;
  Real c3;
  Real c4;
     
// coefficients related to Hw empirical equation
  Real d1;
  Real d2;
  Real d3;
  Real d4;
  Real d5;
  Real d6;
  Real d7;
       
//  VariableValue & _temp_old;
};

#endif //FLUIDFLOW2PHASE_H
