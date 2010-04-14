#include "HeatTransport.h"

template<>
InputParameters validParams<HeatTransport>()
{
  InputParameters params = validParams<PorousMedia>();
  
  params.addParam<Real>("rock_specific_heat",0.92e3,"  //units of (J/(kg K))");
  params.addParam<Real>("thermal_conductivity",2.4,"thermal thermal_conductivity in (W/mK)");
  params.addParam<Real>("water_specific_heat",4.186e3,"units of (J/(kg K))");
  return params;
}

HeatTransport::HeatTransport(std::string name,
                             MooseSystem & moose_system,
                             InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),
     _input_rock_specific_heat(parameters.get<Real>("rock_specific_heat")),
     _input_thermal_conductivity(parameters.get<Real>("thermal_conductivity")),
     _input_water_specific_heat(parameters.get<Real>("water_specific_heat")),     
     //delcare material properties
     _rock_specific_heat(declareRealProperty("rock_specific_heat")),
     _thermal_conductivity(declareRealProperty("thermal_conductivity")),     
     _water_specific_heat(declareRealProperty("water_specific_heat"))
{ }

void
HeatTransport::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
//rock properties
    _rock_specific_heat[qp]  = _input_rock_specific_heat;
    _thermal_conductivity[qp] = _input_thermal_conductivity;
    _water_specific_heat[qp] = _input_water_specific_heat;
  }
}
