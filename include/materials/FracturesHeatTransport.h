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

#ifndef FRACTURESHEATTRANSPORT_H
#define FRACTURESHEATTRANSPORT_H

#include "FracturesPorousMedia.h"


//Forward Declarations
class FracturesHeatTransport;

template<>
InputParameters validParams<FracturesHeatTransport>();

/**
 * Simple material with HeatTransport properties.
 */
class FracturesHeatTransport : virtual public FracturesPorousMedia
{
public:
  FracturesHeatTransport(const std::string & name,
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
  Real _fracture_thermal_conductivity;

////Declare material properties
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _specific_heat_water;
};

#endif //FRACTURESHEATTRANSPORT_H
