#include "HeatTransport.h"

template<>
InputParameters validParams<HeatTransport>()
{
  InputParameters params = validParams<PorousMedia>();
  
  params.addParam<Real>("specific_heat_rock",0.92e3,"  //units of (J/(kg K))");
  params.addParam<Real>("b",2.4,"thermal thermal_conductivity in (W/mK)");
  params.addParam<Real>("specific_heat_water",4.186e3,"units of (J/(kg K))");
  return params;
}

HeatTransport::HeatTransport(const std::string & name,
                             InputParameters parameters)
  :PorousMedia(name, parameters),
     _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
     _input_thermal_conductivity(getParam<Real>("thermal_conductivity")),
     _input_specific_heat_water(getParam<Real>("specific_heat_water")),     
     //declare material properties
     _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
     _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
     _specific_heat_water(declareProperty<Real>("specific_heat_water"))
{ }

void
HeatTransport::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {
//rock properties
    _specific_heat_rock[qp]  = _input_specific_heat_rock;
    _thermal_conductivity[qp] = _input_thermal_conductivity;
    _specific_heat_water[qp] = _input_specific_heat_water;
  }
}
