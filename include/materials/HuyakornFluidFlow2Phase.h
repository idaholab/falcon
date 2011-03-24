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
  Real _input_specific_heat_rock;
  MaterialProperty<Real> & _km;
  MaterialProperty<Real> & _specific_heat_rock;
  
  MaterialProperty<Real> & _enthalpy_saturated_water;
  MaterialProperty<Real> & _enthalpy_saturated_steam;
  MaterialProperty<Real> & _EOS;
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _density_steam;
  MaterialProperty<Real> & _S_water;
  MaterialProperty<Real> & _S_steam;
  MaterialProperty<Real> & _temp;
  MaterialProperty<Real> & _temp_old;
  MaterialProperty<Real> & _Heat;
  MaterialProperty<Real> & _Heat_old;
  MaterialProperty<Real> & _dT_dP;
  MaterialProperty<Real> & _dT_dH;
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _density_old;
  MaterialProperty<Real> & _viscosity_water;
  MaterialProperty<Real> & _viscosity_steam;
  MaterialProperty<Real> & _rel_perm_water;
  MaterialProperty<Real> & _rel_perm_steam;
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<Real> & _tau_steam;
  MaterialProperty<RealGradient> & _darcy_flux_water;
  MaterialProperty<RealGradient> & _darcy_flux_steam;
  MaterialProperty<RealGradient> & _pore_velocity_water;
  MaterialProperty<RealGradient> & _pore_velocity_steam;
  MaterialProperty<Real> & _tau;
  MaterialProperty<Real> & _lamda;
  MaterialProperty<Real> & _beta;
  
   
     Real E3;
     Real E6;
     Real E7;

// coefficients related to density_water empirical equation     
     Real a1;
     Real a2;
     Real a3;
     Real a4;
     Real a5;
     Real a6;

// coefficients related to density_steam empirical equation
     Real b1;
     Real b2;
     Real b3;
     Real b4;
     Real b5;

// coefficients related to enthalpy_saturated_steam empirical equation
     Real c1;
     Real c2;
     Real c3;
     Real c4;
     
// coefficients related to enthalpy_saturated_water empirical equation
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
