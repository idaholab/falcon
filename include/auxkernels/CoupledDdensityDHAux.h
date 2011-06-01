#ifndef COUPLEDDDENSITYDHAUX_H
#define COUPLEDDDENSITYDHAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDdensityDHAux;

template<>
InputParameters validParams<CoupledDdensityDHAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDdensityDHAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDdensityDHAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDdensityDHAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDDENSITYDHAUX_H
