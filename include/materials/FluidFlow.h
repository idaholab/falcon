#ifndef FLUIDFLOW_H
#define FLUIDFLOW_H

#include "PorousMedia.h"


//Forward Declarations
class FluidFlow;

template<>
InputParameters validParams<FluidFlow>();

/**
 * Simple material with FluidFlow properties.
 */
class FluidFlow : virtual public PorousMedia
{
public:
  FluidFlow(const std::string & name,
            InputParameters parameters);
  
protected:
  virtual void computeProperties();

  bool _has_pressure;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;

  bool _has_temp;
  VariableValue  & _temperature;

  Real _input_rho_w;
  Real _input_mu_w;
  Real _input_c_f;

  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _c_f;
  
  bool _has_variable_density;
  MaterialProperty<Real> & _darcy_params_w;
  MaterialProperty<RealGradient> & _darcy_flux_w;
  MaterialProperty<RealGradient> & _pore_velocity_w;
  

};

#endif //FLUIDFLOW_H
