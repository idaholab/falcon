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

#include "FracManGeothermal.h"

template<>
InputParameters validParams<FracManGeothermal>()
{
  InputParameters params = validParams<FracManPorousMedia>();
  params += validParams<FracManFluidFlow>();
  params += validParams<FracManHeatTransport>();
  params += validParams<FracManSolidMechanics>();
  params += validParams<FracManChemicalReactions>();

  return params;
}

FracManGeothermal::FracManGeothermal(const std::string & name,
                       InputParameters parameters)
  :FracManPorousMedia(name, parameters),
   FracManFluidFlow(name, parameters),
   FracManHeatTransport(name, parameters),
   FracManSolidMechanics(name, parameters),
   FracManChemicalReactions(name, parameters)
{}

void
FracManGeothermal::computeProperties()
{
  FracManPorousMedia::computeProperties();
  // Set already computed to true as we compute the intermediate classes' properties
  setPropsComputed(true);

  FracManFluidFlow::computeProperties();
  FracManHeatTransport::computeProperties();
  FracManSolidMechanics::computeProperties();
  FracManChemicalReactions::computeProperties();

  // Now reset this parameter
  setPropsComputed(false);
}
