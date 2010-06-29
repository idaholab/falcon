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
  FluidFlow(std::string name,
            MooseSystem & moose_system,
           InputParameters parameters);
  
protected:
  virtual void computeProperties();
  Real _input_rho_w;
  Real _input_mu_w;
  Real _input_c_f;

  MooseArray<Real> & _rho_w;
  MooseArray<Real> & _mu_w;
  MooseArray<Real> & _c_f;
  
  MooseArray<Real> & _darcy_params_w;
  MooseArray<RealGradient> & _darcy_flux_w;
  MooseArray<RealGradient> & _pore_velocity_w;
  
private:
  bool _has_pressure;
  bool _has_temp;
  bool _has_variable_density;
  MooseArray<Real> & _pressure;
  MooseArray<RealGradient> & _grad_p;
  MooseArray<Real> & _temperature;

};

#endif //FLUIDFLOW_H
