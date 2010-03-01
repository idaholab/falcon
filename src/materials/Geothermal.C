#include "Geothermal.h"

template<>
InputParameters validParams<Geothermal>()
{
  InputParameters params;
  return params;
}

Geothermal::Geothermal(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as)
    :FluidFlow(name,parameters,block_id,coupled_to,coupled_as),
     HeatTransport(name,parameters,block_id,coupled_to,coupled_as),
     SolidMechanics(name,parameters,block_id,coupled_to,coupled_as)
{ }

void
Geothermal::computeProperties()
{
  FluidFlow::computeProperties();
  HeatTransport::computeProperties();
  SolidMechanics::computeProperties();
}
