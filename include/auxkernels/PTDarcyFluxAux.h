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

#ifndef PTDARCYFLUXAUX_H
#define PTDARCYFLUXAUX_H

#include "AuxKernel.h"

class PTDarcyFluxAux;

template<>
InputParameters validParams<PTDarcyFluxAux>();

class PTDarcyFluxAux : public AuxKernel
{
  public:

    PTDarcyFluxAux(const std::string & name, InputParameters parameters);

    virtual ~PTDarcyFluxAux() {}

  protected:

    virtual Real computeValue();

    MaterialProperty<RealGradient> & _wdflx;

    int _i;
};
#endif //PTDARCYFLUXAUX_H
