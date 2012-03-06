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

#include "Geothermal.h"

template<>
InputParameters validParams<Geothermal>()
{
  InputParameters params = validParams<PorousMedia>();
  params += validParams<FluidFlow>();
  params += validParams<HeatTransport>();
  params += validParams<SolidMechanics>();
  return params;
}

Geothermal::Geothermal(const std::string & name,
                       InputParameters parameters)
  :PorousMedia(name, parameters),
   FluidFlow(name, parameters),
   HeatTransport(name, parameters),
   SolidMechanics(name, parameters)
{}

void
Geothermal::computeProperties()
{
  
  FluidFlow::computeProperties();
  HeatTransport::computeProperties();
  SolidMechanics::computeProperties();
}
