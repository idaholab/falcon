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

#include "FracturesGeothermal.h"

template<>
InputParameters validParams<FracturesGeothermal>()
{
  InputParameters params = validParams<FracturesPorousMedia>();
  params += validParams<FracturesFluidFlow>();
  params += validParams<FracturesHeatTransport>();
  params += validParams<FracturesSolidMechanics>();
  params += validParams<FracturesChemicalReactions>();

  return params;
}

FracturesGeothermal::FracturesGeothermal(const std::string & name,
                       InputParameters parameters)
  :FracturesPorousMedia(name, parameters),
   FracturesFluidFlow(name, parameters),
   FracturesHeatTransport(name, parameters),
   FracturesSolidMechanics(name, parameters),
   FracturesChemicalReactions(name, parameters)

{}

void
FracturesGeothermal::computeProperties()
{
  FracturesPorousMedia::computeProperties();
  // Set already computed to true as we compute the intermediate classes' properties
  setPropsComputed(true);

  FracturesFluidFlow::computeProperties();
  FracturesHeatTransport::computeProperties();
  FracturesSolidMechanics::computeProperties();
  FracturesChemicalReactions::computeProperties();

  // Now reset this parameter
  setPropsComputed(false);
}
