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

#ifndef COUPLEDTEMPERATUREAUX_H
#define COUPLEDTEMPERATUREAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledTemperatureAux;

template<>
InputParameters validParams<CoupledTemperatureAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledTemperatureAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledTemperatureAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledTemperatureAux() {}
    
protected:
    virtual Real computeValue();
    Real _input_temperature;
                     
};

#endif //COUPLEDDDENSITYDTAUX_H
