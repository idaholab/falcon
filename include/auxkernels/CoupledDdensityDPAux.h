#ifndef COUPLEDDDENSITYDPAUX_H
#define COUPLEDDDENSITYDPAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDdensityDPAux;

template<>
InputParameters validParams<CoupledDdensityDPAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDdensityDPAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDdensityDPAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDdensityDPAux() {}
    
protected:
    virtual Real computeValue();
  
       
};

#endif //COUPLEDDDENSITYDPAUX_H
