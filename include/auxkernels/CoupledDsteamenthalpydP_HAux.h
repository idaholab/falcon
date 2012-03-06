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

#ifndef COUPLEDDSTEAMENTHALPYDP_HAUX_H
#define COUPLEDDSTEAMENTHALPYDP_HAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDsteamenthalpydP_HAux;

template<>
InputParameters validParams<CoupledDsteamenthalpydP_HAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDsteamenthalpydP_HAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDsteamenthalpydP_HAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDsteamenthalpydP_HAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDSTEAMENTHALPYDP_HAUX_H
