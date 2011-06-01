#ifndef COUPLEDDTDH_PAUX_H
#define COUPLEDDTDH_PAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDTDH_PAux;

template<>
InputParameters validParams<CoupledDTDH_PAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDTDH_PAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDTDH_PAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDTDH_PAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDTDH_PAUX_H
