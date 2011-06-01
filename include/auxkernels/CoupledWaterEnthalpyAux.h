#ifndef COUPLEDWATERENTHALPYAUX_H
#define COUPLEDWATERENTHALPYAUX_H

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledWaterEnthalpyAux;

template<>
InputParameters validParams<CoupledWaterEnthalpyAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledWaterEnthalpyAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledWaterEnthalpyAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledWaterEnthalpyAux() {}
    
protected:
    virtual Real computeValue();
    
    Real _input_enthalpy_water;
    
    
};

#endif //COUPLEDWATERENTHALPYAUX_H
