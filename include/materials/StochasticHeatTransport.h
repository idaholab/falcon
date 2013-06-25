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

#ifndef STOCHASTICHEATTRANSPORT_H
#define STOCHASTICHEATTRANSPORT_H

#include "StochasticPorousMedia.h"


//Forward Declarations
class StochasticHeatTransport;

template<>
InputParameters validParams<StochasticHeatTransport>();

/**
 * Simple material with StochasticHeatTransport properties.
 */
class StochasticHeatTransport : virtual public StochasticPorousMedia
{
public:
  StochasticHeatTransport(const std::string & name,
                InputParameters parameters);
  
protected:
  virtual void computeProperties();
////Grab user input parameters
  Real _input_specific_heat_rock;
  Real _input_thermal_conductivity;
  Real _input_specific_heat_water;

////Declare material properties
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _specific_heat_water;
};

#endif //STOCHASTICHEATTRANSPORT_H
