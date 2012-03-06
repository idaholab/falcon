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

#ifndef PERMEABILITYAUX_H
#define PERMEABILITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class PermeabilityAux;

template<>
InputParameters validParams<PermeabilityAux>();

/** 
 * Coupled auxiliary value
 */
class PermeabilityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  PermeabilityAux(const std::string & name, InputParameters parameters);

  virtual ~PermeabilityAux() {}
  
protected:
  virtual Real computeValue();

  MaterialProperty<Real> & _permeability;

};

#endif //PERMEABILITYAUX_H
