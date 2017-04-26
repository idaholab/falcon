/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef PTPERMEABILITYAUX_H
#define PTPERMEABILITYAUX_H

#include "AuxKernel.h"

class PTPermeabilityAux;

template<>
InputParameters validParams<PTPermeabilityAux>();

class PTPermeabilityAux : public AuxKernel
{
  public:

    PTPermeabilityAux(const InputParameters & parameters);

    virtual ~PTPermeabilityAux() {}

  protected:

    virtual Real computeValue();

    const MaterialProperty<Real> & _perm;
};
#endif //PTPERMEABILITYAUX_H
