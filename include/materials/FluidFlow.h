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
  
  MooseArray<Real> & _darcy_params;
  MooseArray<RealGradient> & _darcy_flux;
  MooseArray<RealGradient> & _pore_velocity;
  
private:
  bool _has_pressure;
  bool _has_temp;
  bool _has_variable_density;
  std::vector<Real> & _pressure;
  std::vector<RealGradient> & _grad_p;
  std::vector<Real> & _temperature;

};

#endif //FLUIDFLOW_H
