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

#include "StochasticGeothermal.h"

template<>
InputParameters validParams<StochasticGeothermal>()
{
  InputParameters params = validParams<StochasticPorousMedia>();
  params += validParams<StochasticFluidFlow>();
  params += validParams<StochasticHeatTransport>();
  params += validParams<StochasticSolidMechanics>();
  params += validParams<StochasticChemicalReactions>();
  return params;
}

StochasticGeothermal::StochasticGeothermal(const std::string & name,
                       InputParameters parameters)
  :StochasticPorousMedia(name, parameters),
   StochasticFluidFlow(name, parameters),
   StochasticHeatTransport(name, parameters),
   StochasticSolidMechanics(name, parameters),
   StochasticChemicalReactions(name, parameters)
{}

void
StochasticGeothermal::computeProperties()
{
  StochasticPorousMedia::computeProperties();
  //Set already computed to true as we compute the intermediate classes' properties
  setPropsComputed(true);
  
  StochasticFluidFlow::computeProperties();
  StochasticHeatTransport::computeProperties();
  StochasticSolidMechanics::computeProperties();
  StochasticChemicalReactions::computeProperties();

  //Now reset this parameter
  setPropsComputed(false);
}
