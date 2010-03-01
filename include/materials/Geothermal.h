#ifndef GEOTHERMAL_H
#define GEOTHERMAL_H

#include "FluidFlow.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"


//Forward Declarations
class Geothermal;

template<>
InputParameters validParams<Geothermal>();

/**
 * Simple material with Geothermal properties.
 */
class Geothermal : public FluidFlow, public HeatTransport, public SolidMechanics
{
public:
  Geothermal(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as);

protected:
  virtual void computeProperties();

};
#endif //GEOTHERMAL_H
