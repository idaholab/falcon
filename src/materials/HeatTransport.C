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

#include "HeatTransport.h"

template<>
InputParameters validParams<HeatTransport>()
{
  InputParameters params = validParams<PorousMedia>();
  params.addCoupledVar("variable_thermal_conductivity", "Variable thermal conductivity from Image Reader");
  params.addParam<Real>("specific_heat_rock",0.92e3,  "specific heat of the rock, [J/(kg.K)]");
  params.addParam<Real>("thermal_conductivity",2.5,"thermal thermal_conductivity, [W/(m.K)]");
  params.addParam<Real>("specific_heat_water",4.186e3,"specific heat of water, [J/(kg.K)]");
  return params;
}

HeatTransport::HeatTransport(const std::string & name,
                             InputParameters parameters)
  :PorousMedia(name, parameters),

     _has_variable_thermal_conductivity(isCoupled("variable_thermal_conductivity")),
     _variable_thermal_conductivity(_has_variable_thermal_conductivity ? coupledValue("variable_thermal_conductivity") : _zero),

////Grab user input parameters
     _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
     _input_thermal_conductivity(getParam<Real>("thermal_conductivity")),
     _input_specific_heat_water(getParam<Real>("specific_heat_water")),

////Declare material properties
     _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
     _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
     _specific_heat_water(declareProperty<Real>("specific_heat_water"))
{ }

void
HeatTransport::computeProperties()
{
//    if (!areParentPropsComputed())
//        PorousMedia::computeProperties();

    if (_has_variable_thermal_conductivity)
      for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
        _thermal_conductivity[qp] = _variable_thermal_conductivity[qp];
    else
      for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
        _thermal_conductivity[qp] = _input_thermal_conductivity;

  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    _specific_heat_rock[qp]  = _input_specific_heat_rock;
    _specific_heat_water[qp] = _input_specific_heat_water;
  }
}
