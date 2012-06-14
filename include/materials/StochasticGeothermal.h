/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef STOCHASTICGEOTHERMAL_H
#define STOCHASTICGEOTHERMAL_H

#include "StochasticFluidFlow.h"
#include "StochasticHeatTransport.h"
#include "StochasticSolidMechanics.h"


//Forward Declarations
class StochasticGeothermal;

template<>
InputParameters validParams<StochasticGeothermal>();

/**
 * Simple material with Geothermal properties.
 */
class StochasticGeothermal : public StochasticFluidFlow, public StochasticHeatTransport, public StochasticSolidMechanics
{
public:
  StochasticGeothermal(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeProperties();

};
#endif //STOCHASTICGEOTHERMAL_H
