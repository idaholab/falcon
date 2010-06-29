#ifndef GEOTHERMAL_H
#define GEOTHERMAL_H

#include "FluidFlow.h"
#include "FluidFlow2Phase.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"


//Forward Declarations
class Geothermal;

template<>
InputParameters validParams<Geothermal>();

/**
 * Simple material with Geothermal properties.
 */
class Geothermal : public FluidFlow2Phase, public HeatTransport, public SolidMechanics
{
public:
  Geothermal(std::string name,
             MooseSystem & moose_system,
             InputParameters parameters);

protected:
  virtual void computeProperties();

};
#endif //GEOTHERMAL_H
