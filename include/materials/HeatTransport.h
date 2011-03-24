#ifndef HEATTRANSPORT_H
#define HEATTRANSPORT_H

#include "PorousMedia.h"


//Forward Declarations
class HeatTransport;

template<>
InputParameters validParams<HeatTransport>();

/**
 * Simple material with HeatTransport properties.
 */
class HeatTransport : virtual public PorousMedia
{
public:
  HeatTransport(const std::string & name,
                InputParameters parameters);
  
protected:
  virtual void computeProperties();

  Real _input_specific_heat_rock;
  Real _input_thermal_conductivity;
  Real _input_specific_heat_water;

  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _specific_heat_water;
};

#endif //HEATTRANSPORT_H
