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
    
    // Real density_fun(Real T);
    
    VariableValue & _temperature;
    VariableValue & _pressure;
    
    //  Real _input_density_water;
    //  bool _has_variable_density;
    //  Real _density_subroutine_val;
    // Real _density_water;
    
    
    
    
};

#endif //COUPLEDDDENSITYDTAUX_H
