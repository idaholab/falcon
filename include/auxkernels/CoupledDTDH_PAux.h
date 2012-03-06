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

#ifndef COUPLEDDTDH_PAUX_H
#define COUPLEDDTDH_PAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDTDH_PAux;

template<>
InputParameters validParams<CoupledDTDH_PAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDTDH_PAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledDTDH_PAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledDTDH_PAux() {}
    
protected:
    virtual Real computeValue();
};

#endif //COUPLEDDTDH_PAUX_H
