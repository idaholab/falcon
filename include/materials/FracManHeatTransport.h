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

#ifndef FRACMANHEATTRANSPORT_H
#define FRACMANHEATTRANSPORT_H

#include "FracManPorousMedia.h"


//Forward Declarations
class FracManHeatTransport;

template<>
InputParameters validParams<FracManHeatTransport>();

/**
 * Simple material with HeatTransport properties.
 */
class FracManHeatTransport : virtual public FracManPorousMedia
{
public:
  FracManHeatTransport(const std::string & name,
                InputParameters parameters);

protected:
  virtual void computeProperties();

////Grab user input parameters
  //General
  Real _input_specific_heat_rock;
  Real _input_specific_heat_water;
  //Matrix
  Real _matrix_thermal_conductivity;
  //Fractures
  std::vector<Real> _fracture_thermal_conductivity_vec;
  std::vector<int> _fracture_number_vec;

////Storing the number of vector entries into respective local variables
  unsigned int num_frac_vec_entries;
  unsigned int num_therm_cond_vec_entries;

////Declare material properties
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _specific_heat_water;
};

#endif //FRACMANHEATTRANSPORT_H
