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
                  InputParameters parameters);
  
protected:
  virtual void computeProperties();

  VariableGradient & _grad_p;
  VariableValue & _pressure;
  VariableValue & _pressure_old;
  VariableValue & _enthalpy;
  VariableValue & _enthalpy_old;

  Real _input_density_water;
  Real _input_density_steam;
  Real _input_compressibility;
  Real _input_viscosity_water;
  Real _input_viscosity_steam;
  
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _density_steam;
  MaterialProperty<Real> & _compressibility;
  MaterialProperty<Real> & _viscosity_water;
  MaterialProperty<Real> & _viscosity_steam;
  MaterialProperty<Real> & _rel_perm_water;
  MaterialProperty<Real> & _rel_perm_steam;

  MaterialProperty<Real> & _temp;
  MaterialProperty<Real> & _temp_old;
  MaterialProperty<Real> & _dTbydP_H;
  MaterialProperty<Real> & _dTbydH_P;
  
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _density_old;
  MaterialProperty<Real> & _ddensitybydP_H;
  MaterialProperty<Real> & _ddensitybydH_P;

  MaterialProperty<Real> & _S_water;
  MaterialProperty<Real> & _S_steam;
  MaterialProperty<Real> & _enthalpy_saturated_water;
  MaterialProperty<Real> & _enthalpy_saturated_steam;
  MaterialProperty<RealGradient> & _Genthalpy_saturated_water;
  MaterialProperty<RealGradient> & _Genthalpy_saturated_steam;
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<Real> & _tau_steam;
  MaterialProperty<RealGradient> & _darcy_flux_water;
  MaterialProperty<RealGradient> & _darcy_flux_steam;

  Real E3;
  Real E6;
  Real E5;
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
       
//  VariableValue & _temp_old;
};

#endif //FLUIDFLOW2PHASE_H
