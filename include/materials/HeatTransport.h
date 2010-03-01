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
class HeatTransport : public PorousMedia
{
public:
  HeatTransport(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as);
  
protected:
  virtual void computeProperties();

  Real _input_rock_specific_heat;
  Real _input_thermal_conductivity;
  Real _input_water_specific_heat;

  std::vector<Real> & _rock_specific_heat;
  std::vector<Real> & _thermal_conductivity;
  std::vector<Real> & _water_specific_heat;
};

#endif //HEATTRANSPORT_H
