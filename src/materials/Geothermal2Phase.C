#include "Geothermal2Phase.h"

template<>
InputParameters validParams<Geothermal2Phase>()
{
  InputParameters params = validParams<PorousMedia>();
  params += validParams<HuyakornFluidFlow2Phase>();
  params += validParams<HeatTransport>();
  params += validParams<SolidMechanics>();
  return params;
}

Geothermal2Phase::Geothermal2Phase(std::string name,
                       MooseSystem & moose_system,
                       InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),
   HuyakornFluidFlow2Phase(name, moose_system, parameters),
   HeatTransport(name, moose_system, parameters),
   SolidMechanics(name, moose_system, parameters)
{}

void
Geothermal2Phase::computeProperties()
{
  
  HuyakornFluidFlow2Phase::computeProperties();
  HeatTransport::computeProperties();
  SolidMechanics::computeProperties();
}
