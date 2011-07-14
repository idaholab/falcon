#ifndef COUPLEDDWATERSATURATIONDHAUX
#define COUPLEDDWATERSATURATIONDHAUX

#include "AuxKernel.h"


//Forward Declarations
class CoupledDWaterSaturationDHAux;

template<>
InputParameters validParams<CoupledDWaterSaturationDHAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDWaterSaturationDHAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDWaterSaturationDHAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDWaterSaturationDHAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDWATERSATURATIONDHAUX
