#ifndef COUPLEDDDENSITYDPAUX_H_PT
#define COUPLEDDDENSITYDPAUX_H_PT

#include "AuxKernel.h"


//Forward Declarations
class CoupledDdensityDPAux_PT;

template<>
InputParameters validParams<CoupledDdensityDPAux_PT>();

/** 
 * Coupled auxiliary value
 */
class CoupledDdensityDPAux_PT : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDdensityDPAux_PT(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDdensityDPAux_PT() {}
    
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

#endif //COUPLEDDDENSITYDTAUX_H_PT
