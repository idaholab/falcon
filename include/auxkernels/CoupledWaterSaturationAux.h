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

#ifndef COUPLEDWATERSATURATIONAUX_H
#define COUPLEDWATERSATURATIONAUX_H

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledWaterSaturationAux;

template<>
InputParameters validParams<CoupledWaterSaturationAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledWaterSaturationAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledWaterSaturationAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledWaterSaturationAux() {}
    
protected:
    virtual Real computeValue();
    
    Real _input_saturation_water;
    
    
};

#endif //COUPLEDWATERSATURATIONAUX_H
