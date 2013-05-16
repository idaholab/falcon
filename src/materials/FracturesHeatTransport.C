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
  
    params.addParam<Real>("fracture_num", 0, "number in fracture map that indicates fractures");
    params.addParam<Real>("matrix_num", 255, "number in fracture map that indicates matrix");
    params.addParam<Real>("specific_heat_rock",0.92e3,  "units of J/(kg.K)");
    params.addParam<Real>("specific_heat_water",4.186e3,"units of J/(kg.K)");
    params.addParam<Real>("matrix_thermal_conductivity", 2.5, "thermal conductivity of matrix in W/(m.K)");
    params.addParam<Real>("fracture_thermal_conductivity", 2.5, "thermal conductivity of fractures in W/(m.K)");
  
  return params;
}

FracturesHeatTransport::FracturesHeatTransport(const std::string & name,
                             InputParameters parameters)
  :FracturesPorousMedia(name, parameters),

    _fracture_num(getParam<Real>("fracture_num")),
    _matrix_num(getParam<Real>("matrix_num")),

    _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
    _input_specific_heat_water(getParam<Real>("specific_heat_water")),

    _matrix_thermal_conductivity(getParam<Real>("matrix_thermal_conductivity")),
    _fracture_thermal_conductivity(getParam<Real>("fracture_thermal_conductivity")),

    //declare material properties
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
      
      //rock properties
      _specific_heat_rock[qp]  = _input_specific_heat_rock;
      _specific_heat_water[qp] = _input_specific_heat_water;

      if (_fractures[qp] == _matrix_num)
          _thermal_conductivity[qp] = _matrix_thermal_conductivity;
      else if (_fractures[qp] == _fracture_num)
          _thermal_conductivity[qp] = _fracture_thermal_conductivity;
      else
          _thermal_conductivity[qp] = _matrix_thermal_conductivity;
  }
}
