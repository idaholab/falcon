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

#ifndef COUPLEDSTEAMENTHALPYAUX_H
#define COUPLEDSTEAMENTHALPYAUX_H

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledSteamEnthalpyAux;

template<>
InputParameters validParams<CoupledSteamEnthalpyAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledSteamEnthalpyAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledSteamEnthalpyAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledSteamEnthalpyAux() {}
    
protected:
    virtual Real computeValue();
    
    Real _input_enthalpy_steam;
    
    
};

#endif //COUPLEDSTEAMENTHALPYAUX_H
