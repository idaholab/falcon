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
  HeatTransport(std::string name,
                MooseSystem & moose_system,
                InputParameters parameters);
  
protected:
  virtual void computeProperties();

  Real _input_rock_specific_heat;
  Real _input_thermal_conductivity;
  Real _input_water_specific_heat;

  MooseArray<Real> & _rock_specific_heat;
  MooseArray<Real> & _thermal_conductivity;
  MooseArray<Real> & _water_specific_heat;
};

#endif //HEATTRANSPORT_H
