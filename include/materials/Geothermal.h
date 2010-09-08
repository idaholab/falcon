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
  Geothermal(const std::string & name,
             MooseSystem & moose_system,
             InputParameters parameters);

protected:
  virtual void computeProperties();

};
#endif //GEOTHERMAL_H
