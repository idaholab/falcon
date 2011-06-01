#ifndef COUPLEDWATERDENSITYAUX_H
#define COUPLEDWATERDENSITYAUX_H

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledWaterDensityAux;

template<>
InputParameters validParams<CoupledWaterDensityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledWaterDensityAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledWaterDensityAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledWaterDensityAux() {}
    
protected:
    virtual Real computeValue();
    
    Real _input_density_water;
    
    
};

#endif //COUPLEDWATERDENSITYAUX_H
