#ifndef COUPLEDDTDP_HAUX_H
#define COUPLEDDTDP_HAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDTDP_HAux;

template<>
InputParameters validParams<CoupledDTDP_HAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDTDP_HAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDTDP_HAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDTDP_HAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDTDP_HAUX_H
