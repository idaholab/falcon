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

#include "FracManHeatTransport.h"

template<>
InputParameters validParams<FracManHeatTransport>()
{
  InputParameters params = validParams<FracManPorousMedia>();
  
    params.addParam<Real>("specific_heat_rock",0.92e3,  "units of J/(kg.K)");
    params.addParam<Real>("specific_heat_water",4.186e3,"units of J/(kg.K)");
    params.addParam<Real>("matrix_thermal_conductivity",2.5,"thermal conductivity of rock matrix W/(m.K)");
    params.addRequiredParam<std::vector<Real> >("fracture_thermal_conductivities","thermal conductivity values associated with each of the fractures W/(m.K)");
    params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from the FracMan file");
    
  return params;
}

FracManHeatTransport::FracManHeatTransport(const std::string & name,
                             InputParameters parameters)
  :FracManPorousMedia(name, parameters),
     _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
     _input_specific_heat_water(getParam<Real>("specific_heat_water")),

_matrix_thermal_conductivity(getParam<Real>("matrix_thermal_conductivity")),
_fracture_thermal_conductivity_vec(getParam<std::vector<Real> >("fracture_thermal_conductivities")),
_fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),

    //declare material properties
     _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
     _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
     _specific_heat_water(declareProperty<Real>("specific_heat_water"))
{
    num_frac_vec_entries = _fracture_number_vec.size();
    num_therm_cond_vec_entries = _fracture_thermal_conductivity_vec.size();
}

void
FracManHeatTransport::computeProperties()
{
  if (!areParentPropsComputed())
    FracManPorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      if (_fracture_map[qp] == 0)
      {
          _thermal_conductivity[qp]         = _matrix_thermal_conductivity;
      }
      
      for (unsigned int k = 0; k < num_frac_vec_entries; k++)
      {
          if (_fracture_map[qp] == _fracture_number_vec[k])
          {
              if (num_therm_cond_vec_entries < 2)
                  _thermal_conductivity[qp] = _fracture_thermal_conductivity_vec[0];
              else
                  _thermal_conductivity[qp] = _fracture_thermal_conductivity_vec[k];
          }
      }
      
    _specific_heat_rock[qp]  = _input_specific_heat_rock;
    _specific_heat_water[qp] = _input_specific_heat_water;
  }
}
