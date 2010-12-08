#include "Geothermal2Phase.h"

template<>
InputParameters validParams<Geothermal2Phase>()
{
  InputParameters params = validParams<PorousMedia>();
  params += validParams<FluidFlow2Phase>();
  params += validParams<HeatTransport>();
  params += validParams<SolidMechanics>();
  return params;
}

Geothermal2Phase::Geothermal2Phase(const std::string & name,
                       InputParameters parameters)
  :PorousMedia(name, parameters),
   FluidFlow2Phase(name, parameters),
   HeatTransport(name, parameters),
   SolidMechanics(name, parameters)
{}

void
Geothermal2Phase::computeProperties()
{
  
  FluidFlow2Phase::computeProperties();
  HeatTransport::computeProperties();
  SolidMechanics::computeProperties();
}
