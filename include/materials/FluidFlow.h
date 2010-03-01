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
class FluidFlow : public PorousMedia
{
public:
  FluidFlow(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as);
  
protected:
  virtual void computeProperties();
  Real _input_rho_w;
  Real _input_mu_w;
  Real _input_c_f;

  std::vector<Real> & _rho_w;
  std::vector<Real> & _mu_w;
  std::vector<Real> & _c_f;
  
  std::vector<Real> & _darcy_params;
  std::vector<RealGradient> & _darcy_flux;
  std::vector<RealGradient> & _pore_velocity;
  
private:
  bool _has_pressure;
  bool _has_temp;
  bool _has_variable_density;
  std::vector<Real> & _pressure;
  std::vector<RealGradient> & _grad_p;
  std::vector<Real> & _temperature;

};

#endif //FLUIDFLOW_H
