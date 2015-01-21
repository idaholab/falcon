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
  ////Genreal
  params.addParam<Real>("specific_heat_rock", 920,  "specific heat of the rock, [J/(kg.K)]");
  params.addParam<Real>("specific_heat_water",4.186e3,"specific heat of water, [J/(kg.K)]");
  ////Matrix
  params.addParam<Real>("matrix_thermal_conductivity",2.5, "thermal conductivity of matrix rock, [W/(m.K)]");
  ////Fractures
  params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from the FracMan file");
  params.addRequiredParam<std::vector<Real> >("fracture_thermal_conductivities","thermal conductivity values associated with each of the fractures, [W/(m.K)]");


  return params;
}

FracManHeatTransport::FracManHeatTransport(const std::string & name,
                             InputParameters parameters)
  :FracManPorousMedia(name, parameters),
////Grab user input parameters
    ////General
     _input_specific_heat_rock(getParam<Real>("specific_heat_rock")),
     _input_specific_heat_water(getParam<Real>("specific_heat_water")),
    ////Matrix
    _matrix_thermal_conductivity(getParam<Real>("matrix_thermal_conductivity")),
    ////Fractures
    _fracture_thermal_conductivity_vec(getParam<std::vector<Real> >("fracture_thermal_conductivities")),
    _fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),

////Declare material properties
     _specific_heat_rock(declareProperty<Real>("specific_heat_rock")),
     _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
     _specific_heat_water(declareProperty<Real>("specific_heat_water"))

{
    // storing the number of vector entries into respective local variables
    num_frac_vec_entries        = _fracture_number_vec.size();
    num_therm_cond_vec_entries  = _fracture_thermal_conductivity_vec.size();

    // we want either one value of thermal_conductivity to assign to all fractures or an individual value to assign to each fracture
    if (((num_therm_cond_vec_entries > 2) && (num_therm_cond_vec_entries < num_frac_vec_entries)) || (num_therm_cond_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one thermal conductivity value for all fractures or a thermal conductivity value for each fracture");
}

void
FracManHeatTransport::computeProperties()
{
  if (!areParentPropsComputed())
    FracManPorousMedia::computeProperties();

  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      //material property assignment for matrix
      if (_fracture_map[qp] == 0)
      {
          _thermal_conductivity[qp]         = _matrix_thermal_conductivity;
      }
      //material property assignment for each of the fractures
      for (unsigned int k = 0; k < num_frac_vec_entries; k++)
      {
          //If only one thermal_conductivity value was provided, assign this value to all fractures
          //Else cycle through each fracture and assign values in order provided
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
