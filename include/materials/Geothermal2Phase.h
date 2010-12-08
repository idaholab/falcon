#ifndef GEOTHERMAL2PHASE_H
#define GEOTHERMAL2PHASE_H

#include "FluidFlow2Phase.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"


//Forward Declarations
class Geothermal2Phase;

template<>
InputParameters validParams<Geothermal2Phase>();

/**
 * Simple material with Geothermal2Phase properties.
 */
class Geothermal2Phase : public FluidFlow2Phase, public HeatTransport, public SolidMechanics
{
public:
  Geothermal2Phase(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeProperties();

};
#endif //GEOTHERMAL2PHASE_H
