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

#ifndef FRACTURESGEOTHERMAL_H
#define FRACTURESGEOTHERMAL_H

#include "FracturesFluidFlow.h"
#include "FracturesHeatTransport.h"
#include "FracturesSolidMechanics.h"
#include "FracturesChemicalReactions.h"


//Forward Declarations
class FracturesGeothermal;

template<>
InputParameters validParams<FracturesGeothermal>();

/**
 * Simple material with Geothermal properties.
 */
class FracturesGeothermal : public FracturesFluidFlow, public FracturesHeatTransport, public FracturesSolidMechanics, public FracturesChemicalReactions
{
public:
  FracturesGeothermal(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeProperties();
    
};
#endif //FRACTURESGEOTHERMAL_H
