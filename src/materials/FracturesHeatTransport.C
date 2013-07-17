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

#include "FracturesHeatTransport.h"

template<>
InputParameters validParams<FracturesHeatTransport>()
{
  InputParameters params = validParams<FracturesPorousMedia>();
  ////General
    params.addParam<Real>("specific_heat_rock",0.92e3,  "specific heat of the rock, [J/(kg.K)]");
    params.addParam<Real>("specific_heat_water",4.186e3,"specific heat of water, [J/(kg.K)]");
  ////Matrix
    params.addParam<Real>("matrix_thermal_conductivity", 2.5, "thermal conductivity of matrix rock, [W/(m.K)]");
  ////Fractures
    params.addParam<Real>("fracture_thermal_conductivity", 2.5, "thermal conductivity of fractures, [W/(m.K)]");
  
  return params;
}

FracturesHeatTransport::FracturesHeatTransport(const std::string & name,
                             InputParameters parameters)
  :FracturesPorousMedia(name, parameters),
////Grab user input parameters
    ////General
    _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
    _input_specific_heat_water(getParam<Real>("specific_heat_water")),
    ////Matrix
    _matrix_thermal_conductivity(getParam<Real>("matrix_thermal_conductivity")),
    ////Fractures
    _fracture_thermal_conductivity(getParam<Real>("fracture_thermal_conductivity")),

////Declare material properties
    _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
    _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
    _specific_heat_water(declareProperty<Real>("specific_heat_water"))

{ }

void
FracturesHeatTransport::computeProperties()
{
  if (!areParentPropsComputed())
    FracturesPorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      _specific_heat_rock[qp]  = _input_specific_heat_rock;
      _specific_heat_water[qp] = _input_specific_heat_water;

      //material property assignment for the fractures
      if (_fractures[qp] == _fracture_num)
          _thermal_conductivity[qp] = _fracture_thermal_conductivity;
      //material property assignment for matrix
      else
          _thermal_conductivity[qp] = _matrix_thermal_conductivity;
  }
}
