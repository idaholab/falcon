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
