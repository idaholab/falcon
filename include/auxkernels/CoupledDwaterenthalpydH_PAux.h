#ifndef COUPLEDDWATERENTHALPYDH_PAux
#define COUPLEDDWATERENTHALPYDH_PAux

#include "AuxKernel.h"


//Forward Declarations
class CoupledDwaterenthalpydH_PAux;

template<>
InputParameters validParams<CoupledDwaterenthalpydH_PAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDwaterenthalpydH_PAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDwaterenthalpydH_PAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDwaterenthalpydH_PAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDWATERENTHALPYDH_PAux
