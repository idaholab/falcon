/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef COUPLEDSTEAMVISCOSITYAUX_H
#define COUPLEDSTEAMVISCOSITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledSteamViscosityAux;
class WaterSteamEOS;

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
    const WaterSteamEOS & _water_steam_properties;
    
    VariableValue & _density_steam;
    VariableValue & _temperature;
    Real _input_viscosity_steam;
    bool _has_variable_viscosity;
  
    
};

#endif //COUPLEDSTEAMVISCOSITYAUX_H
