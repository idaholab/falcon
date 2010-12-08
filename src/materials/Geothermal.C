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
