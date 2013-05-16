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

  Real _fracture_num;
  Real _matrix_num;
    
  Real _input_specific_heat_rock;
  Real _input_specific_heat_water;
    
  Real _matrix_thermal_conductivity;
    
  Real _fracture_thermal_conductivity;

  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _specific_heat_water;
};

#endif //FRACTURESHEATTRANSPORT_H
