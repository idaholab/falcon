#ifndef HUYAKORNFLUIDFLOW2PHASE_H
#define HUYAKORNFLUIDFLOW2PHASE_H

#include "PorousMedia.h"


//Forward Declarations
class HuyakornFluidFlow2Phase;

template<>
InputParameters validParams<HuyakornFluidFlow2Phase>();

/**
 * Simple material with HuyakornFluidFlow2Phase properties.
 */
class HuyakornFluidFlow2Phase : virtual public PorousMedia
{
public:
  HuyakornFluidFlow2Phase(const std::string & name,
           InputParameters parameters);
  
protected:
  virtual void computeProperties();

  VariableValue & _pressure;
  VariableValue & _pressure_old;
  VariableGradient & _grad_p;
  VariableValue & _enthalpy;
  VariableValue & _enthalpy_old;


  Real _input_thermal_conductivity;
  Real _input_cp_r;
  MaterialProperty<Real> & _km;
  MaterialProperty<Real> & _cp_r;
  
  MaterialProperty<Real> & _Hw;
  MaterialProperty<Real> & _Hs;
  MaterialProperty<Real> & _EOS;
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _rho_s;
  MaterialProperty<Real> & _sat_w;
  MaterialProperty<Real> & _sat_s;
  MaterialProperty<Real> & _temp;
  MaterialProperty<Real> & _temp_old;
  MaterialProperty<Real> & _Heat;
  MaterialProperty<Real> & _Heat_old;
  MaterialProperty<Real> & _dT_dP;
  MaterialProperty<Real> & _dT_dH;
  MaterialProperty<Real> & _rho;
  MaterialProperty<Real> & _rho_old;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _mu_s;
  MaterialProperty<Real> & _rel_perm_w;
  MaterialProperty<Real> & _rel_perm_s;
  MaterialProperty<Real> & _darcy_params_w;
  MaterialProperty<Real> & _darcy_params_s;
  MaterialProperty<RealGradient> & _darcy_flux_w;
  MaterialProperty<RealGradient> & _darcy_flux_s;
  MaterialProperty<RealGradient> & _pore_velocity_w;
  MaterialProperty<RealGradient> & _pore_velocity_s;
  MaterialProperty<Real> & _tau;
  MaterialProperty<Real> & _lamda;
  MaterialProperty<Real> & _beta;
  
   
     Real E3;
     Real E6;
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

// coefficients related to Tw empirical equation
     Real w1;
     Real w2;
     Real w3;
     Real w4;
     Real w5;
     
// coefficients related to Ts empirical equation
     Real s1;
     Real s2;
     Real s3;
     Real s4;
     Real s5;
     Real s6;
     Real s7;
     Real s8;

  
};

#endif //HUYAKORNFLUIDFLOW2PHASE_H
