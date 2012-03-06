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
