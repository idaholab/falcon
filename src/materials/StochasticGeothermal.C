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
  return params;
}

StochasticGeothermal::StochasticGeothermal(const std::string & name,
                       InputParameters parameters)
  :StochasticPorousMedia(name, parameters),
   StochasticFluidFlow(name, parameters),
   StochasticHeatTransport(name, parameters),
   StochasticSolidMechanics(name, parameters)
{}

void
StochasticGeothermal::computeProperties()
{
  
  StochasticFluidFlow::computeProperties();
  StochasticHeatTransport::computeProperties();
  StochasticSolidMechanics::computeProperties();
}
