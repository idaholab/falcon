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

Geothermal::Geothermal(std::string name,
                       MooseSystem & moose_system,
                       InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),
   FluidFlow(name, moose_system, parameters),
   HeatTransport(name, moose_system, parameters),
   SolidMechanics(name, moose_system, parameters)
{}

void
Geothermal::computeProperties()
{
  FluidFlow::computeProperties();
  HeatTransport::computeProperties();
  SolidMechanics::computeProperties();
}
