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

#ifndef PTDARCYFLUXAUX_H
#define PTDARCYFLUXAUX_H

#include "AuxKernel.h"

class PTDarcyFluxAux : public AuxKernel
{
  public:

    PTDarcyFluxAux(const InputParameters & parameters);

    virtual ~PTDarcyFluxAux() {}

  protected:

    virtual Real computeValue();

    const MaterialProperty<RealGradient> & _wdflx;

    int _i;

  public:
    static InputParameters validParams();
};

#endif //PTDARCYFLUXAUX_H
