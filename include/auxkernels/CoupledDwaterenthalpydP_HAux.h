#ifndef COUPLEDDWATERENTHALPYDP_HAUX_H
#define COUPLEDDWATERENTHALPYDP_HAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDwaterenthalpydP_HAux;

template<>
InputParameters validParams<CoupledDwaterenthalpydP_HAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDwaterenthalpydP_HAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDwaterenthalpydP_HAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDwaterenthalpydP_HAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDWATERENTHALPYDP_HAUX_H
