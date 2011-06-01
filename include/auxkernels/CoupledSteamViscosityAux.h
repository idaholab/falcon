#ifndef COUPLEDSTEAMVISCOSITYAUX_H
#define COUPLEDSTEAMVISCOSITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledSteamViscosityAux;

template<>
InputParameters validParams<CoupledSteamViscosityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledSteamViscosityAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledSteamViscosityAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledSteamViscosityAux() {}
    
protected:
    virtual Real computeValue();
    
    // Real viscosity_fun(Real T);
    VariableValue & _density_steam;
    VariableValue & _temperature;
    Real _input_viscosity_steam;
    bool _has_variable_viscosity;
  
    
};

#endif //COUPLEDSTEAMVISCOSITYAUX_H
