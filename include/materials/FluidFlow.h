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

  Real density_fun(Real T);
  Real viscosity_fun(Real T);
  
  bool _has_pressure;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;

  bool _has_temp;
  VariableValue  & _temperature;

  Real _input_density_water;
  Real _input_viscosity_water;
  Real _input_compressibility;
  Real _constant_temperature;

  
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _viscosity_water;
  MaterialProperty<Real> & _compressibility;
  
   bool _has_variable_density;
  //bool _temp_dependant_density;
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<RealGradient> & _darcy_flux_water;
  MaterialProperty<RealGradient> & _pore_velocity_water;
  

};

#endif //FLUIDFLOW_H
