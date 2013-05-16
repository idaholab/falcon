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

#ifndef FRACTURESGEOTHERMAL_H
#define FRACTURESGEOTHERMAL_H

#include "FracturesFluidFlow.h"
#include "FracturesHeatTransport.h"
#include "FracturesSolidMechanics.h"


//Forward Declarations
class FracturesGeothermal;

template<>
InputParameters validParams<FracturesGeothermal>();

/**
 * Simple material with Geothermal properties.
 */
class FracturesGeothermal : public FracturesFluidFlow, public FracturesHeatTransport, public FracturesSolidMechanics
{
public:
  FracturesGeothermal(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeProperties();
    
    Real _fracture_num;
    Real _matrix_num;
    
    bool _has_strain_change_permeability;
    Real _model_fracture_aperture;
    MaterialProperty<Real> & _eq_aperture;
    
};
#endif //FRACTURESGEOTHERMAL_H
