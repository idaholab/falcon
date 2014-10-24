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

#ifndef FRACMANGEOTHERMAL_H
#define FRACMANGEOTHERMAL_H

#include "FracManFluidFlow.h"
#include "FracManHeatTransport.h"
#include "FracManSolidMechanics.h"
#include "FracManChemicalReactions.h"

//Forward Declarations
class FracManGeothermal;

template<>
InputParameters validParams<FracManGeothermal>();

/**
 * Simple material with Geothermal properties.
 */
class FracManGeothermal : public FracManFluidFlow, public FracManHeatTransport, public FracManSolidMechanics, public FracManChemicalReactions
{
public:
  FracManGeothermal(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeProperties();
};
#endif //FRACMANGEOTHERMAL_H
