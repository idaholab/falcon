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
